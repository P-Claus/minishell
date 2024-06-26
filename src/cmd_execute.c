/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_execute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pclaus <pclaus@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 22:08:47 by efret             #+#    #+#             */
/*   Updated: 2024/06/18 16:23:18 by efret            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	open_file(char *name, int flag)
{
	int	fd;

	if (flag == R_IN)
		fd = open(name, O_RDONLY | O_CLOEXEC);
	else if (flag == R_OUT)
		fd = open(name, O_WRONLY | O_TRUNC | O_CLOEXEC | O_CREAT, 00666);
	else if (flag == R_APND)
		fd = open(name, O_WRONLY | O_APPEND | O_CLOEXEC | O_CREAT, 00666);
	else
		exit(1);
	return (fd);
}

int	read_here_doc(int pipe_fd[2], t_redir *redir)
{
	char	*line;

	if (pipe(pipe_fd) == -1)
		exit_handler(1); // Error handling
	line = readline("> ");
	while (line)
	{
		if (exact_match(line, redir->str))
		{
			free(line);
			break ;
		}
		write(pipe_fd[PIPE_W], line, ft_strlen(line));
		write(pipe_fd[PIPE_W], "\n", 1);
		free(line);
		line = readline("> ");
	}
	close(pipe_fd[PIPE_W]);
	return (pipe_fd[PIPE_R]);
}

void	parse_here_docs(t_cmd *cmds, int pipe_fd[2])
{
	t_redir	*redirs;

	while (cmds)
	{
		redirs = cmds->redirs;
		while (redirs)
		{
			if (redirs->flags == R_HERE)
			{
				redirs->fd = read_here_doc(pipe_fd, redirs);
				redirs->is_fd = true;
			}
			redirs = redirs->next;
		}
		cmds = cmds->next;
	}
}

void	do_redirs(t_cmd *cmd)
{
	t_redir	*redirs;
	int		fd;

	redirs = cmd->redirs;
	while (redirs)
	{
		if (redirs->flags == R_HERE && redirs->is_fd)
		{
			if (dup2(redirs->fd, STDIN_FILENO) == -1)
				exit_handler(1);
			close(redirs->fd);
		}
		else
		{
			fd = open_file(redirs->str, redirs->flags);
			if (errno)
				exit_handler(1);
			if (redirs->flags == R_IN)
			{
				redirs->fd = fd;
				redirs->is_fd = true;
				if (dup2(fd, STDIN_FILENO) == -1)
					exit_handler(1);
			}
			else
			{
				redirs->fd = fd;
				redirs->is_fd = true;
				if (dup2(fd, STDOUT_FILENO) == -1)
					exit_handler(1);
			}
		}
		redirs = redirs->next;
	}
}

void	close_redirs(t_cmd *cmd)
{
	t_redir	*redirs;

	redirs = cmd->redirs;
	while (redirs)
	{
		if (redirs->flags)
			; // No need to do anything, pipe end should be closed already.
		else
		{
			close(redirs->fd);
			redirs->fd = false;
		}
		redirs = redirs->next;
	}
}

static void	ft_execve(t_cmd *cmd, int pipe_fd[2], t_minishell *shell)
{
	char	*cmd_path;

	handle_sigquit_child();
	if (cmd->next && dup2(pipe_fd[PIPE_W], STDOUT_FILENO) == -1)
		exit_handler(1); // error
	close(pipe_fd[PIPE_W]);
	do_redirs(cmd);
	cmd_path = cmd_find_path(cmd->cmd_av[0], shell->env);
	if (!cmd_path)
		(printf("CMD NOT FOUND\n"), exit_handler(1));
	execve(cmd_path, cmd->cmd_av, shell->export_env);
	exit_handler(1); // reached if execve (execpv) had an error.
}

static inline void	status_stuff(pid_t cpid, pid_t pid, int wstat)
{
	if (pid != cpid)
		return ;
	if (WIFEXITED(wstat))
		g_shell_stats.prev_exit = WEXITSTATUS(wstat);
	else if (WIFSIGNALED(wstat))
	{
		if (WCOREDUMP(wstat))
		{
			printf("Quit (core dumped)\n");
			g_shell_stats.prev_exit = ENOTRECOVERABLE;
		}
		else if (WTERMSIG(wstat) == CLD_KILLED)
		{
			write(1, "\n", 1);
			g_shell_stats.prev_exit = EOWNERDEAD;
		}
	}
}

void	ft_wait(pid_t cpid)
{
	int		wstat;
	pid_t	pid;

	while ((pid = wait(&wstat)) > 0)
		status_stuff(cpid, pid, wstat);
	if (errno == EINTR)
	{
		pid = waitpid(cpid, &wstat, 0);
		status_stuff(cpid, pid, wstat);
	}
	g_shell_stats.process_is_running = 0;
}

void	ft_run_cmds(t_cmd *cmds, t_minishell *shell)
{
	int	pipe_fd[2];
	int	cpid;
	int	stdin_copy;

	// copy to restore stdin for later. Maybe need to do this for the other std streams as well.
	parse_here_docs(cmds, pipe_fd);
	stdin_copy = dup(STDIN_FILENO);
	g_shell_stats.prev_exit = 0;
	g_shell_stats.process_is_running = 1;
	while (cmds && g_shell_stats.process_is_running)
	{
		if (pipe(pipe_fd) == -1)
			exit_handler(1);
		if (check_for_builtins(cmds, shell, pipe_fd))
		{
			if (!g_shell_stats.prev_exit)
				cmds = cmds->next;
			else
				break ;
			continue;
		}
		cpid = fork();
		if (cpid == -1)
			exit_handler(1);
		if (!cpid)
			ft_execve(cmds, pipe_fd, shell);
		g_shell_stats.cmd_pid = cpid;
		close(pipe_fd[PIPE_W]);
		if (dup2(pipe_fd[PIPE_R], STDIN_FILENO) == -1)
			exit_handler(1);
		close(pipe_fd[PIPE_R]);
		cmds = cmds->next;
	}
	ft_wait(cpid);
	if (dup2(stdin_copy, STDIN_FILENO) == -1 || close(stdin_copy))
		exit_handler(1);
}
