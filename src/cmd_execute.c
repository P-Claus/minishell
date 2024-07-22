/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_execute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pclaus <pclaus@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 22:08:47 by efret             #+#    #+#             */
/*   Updated: 2024/07/22 19:16:43 by efret            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

	pid = wait(&wstat);
	while (pid > 0)
	{
		status_stuff(cpid, pid, wstat);
		pid = wait(&wstat);
	}
	if (errno == EINTR)
	{
		pid = waitpid(cpid, &wstat, 0);
		status_stuff(cpid, pid, wstat);
	}
	else if (errno == ECHILD)
	{
		errno = 0;
	}
	g_shell_stats.process_is_running = 0;
}

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
				old_exit_handler(1);
			close(redirs->fd);
		}
		else
		{
			fd = open_file(redirs->str, redirs->flags);
			if (errno)
				old_exit_handler(1);
			if (redirs->flags == R_IN)
			{
				redirs->fd = fd;
				redirs->is_fd = true;
				if (dup2(fd, STDIN_FILENO) == -1)
					old_exit_handler(1);
			}
			else
			{
				redirs->fd = fd;
				redirs->is_fd = true;
				if (dup2(fd, STDOUT_FILENO) == -1)
					old_exit_handler(1);
			}
		}
		redirs = redirs->next;
	}
}

void	close_redirs(t_cmd *cmds)
{
	t_cmd	*cmd;
	t_redir	*redirs;

	cmd = cmds;
	while (cmd)
	{
		redirs = cmd->redirs;
		while (redirs)
		{
			if (redirs->fd > 0)
			{
				close(redirs->fd);
				redirs->is_fd = false;
			}
			redirs = redirs->next;
		}
		cmd = cmd->next;
	}
}

static void	ft_execve(t_cmd *cmd, int pipe_fd[2], t_minishell *shell)
{
	char	*cmd_path;

	handle_sigquit_child();
	if (cmd->next && dup2(pipe_fd[PIPE_W], STDOUT_FILENO) == -1)
		old_exit_handler(1); // error
	close(pipe_fd[PIPE_W]);
	do_redirs(cmd);
	check_for_leading_vars(cmd, shell);
	if (!cmd->cmd_av[0] || !cmd->cmd_av[0][0])
		exit_handler(shell, -1);
	cmd_path = cmd_find_path(cmd->cmd_av[0], shell->env);
	if (!cmd_path)
		(printf("CMD NOT FOUND\n"), old_exit_handler(1));
	execve(cmd_path, cmd->cmd_av, shell->export_env);
	old_exit_handler(1); // reached if execve (execpv) had an error.
}

void	ft_run_cmds(t_cmd *cmds, t_minishell *shell)
{
	int	pipe_fd[2];
	int	cpid;
	int	stdin_copy;

	// copy to restore stdin for later. Maybe need to do this for the other std streams as well.
	g_shell_stats.prev_exit = 0;
	check_for_exit(cmds, shell);
	if (g_shell_stats.prev_exit)
		return ;
	if (check_for_only_vars(cmds, shell))
		return ;
	parse_here_docs(shell, cmds, pipe_fd);
	if (g_shell_stats.prev_exit)
		return ;
	stdin_copy = dup(STDIN_FILENO);
	g_shell_stats.process_is_running = 1;
	while (cmds && g_shell_stats.process_is_running)
	{
		if (pipe(pipe_fd) == -1)
			old_exit_handler(1);
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
			old_exit_handler(1);
		if (!cpid)
			ft_execve(cmds, pipe_fd, shell);
		g_shell_stats.cmd_pid = cpid;
		close(pipe_fd[PIPE_W]);
		if (dup2(pipe_fd[PIPE_R], STDIN_FILENO) == -1)
			old_exit_handler(1);
		close(pipe_fd[PIPE_R]);
		cmds = cmds->next;
	}
	ft_wait(cpid);
	if (dup2(stdin_copy, STDIN_FILENO) == -1 || close(stdin_copy))
		old_exit_handler(1);
}
