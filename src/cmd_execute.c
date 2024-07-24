/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_execute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pclaus <pclaus@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 22:08:47 by efret             #+#    #+#             */
/*   Updated: 2024/07/24 16:25:05 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static inline void	ft_execve(t_cmd *cmd, int pipe_fd[2], t_minishell *shell)
{
	char	*cmd_path;

	handle_sigquit_child();
	if (cmd->next && dup2(pipe_fd[PIPE_W], STDOUT_FILENO) == -1)
		exit_handler(shell, errno);
	close(pipe_fd[PIPE_W]);
	do_redirs(cmd, shell);
	check_for_leading_vars(cmd, shell);
	if (!cmd->cmd_av[0] || !cmd->cmd_av[0][0])
		exit_handler(shell, -1);
	cmd_path = cmd_find_path(cmd->cmd_av[0], shell->env, shell);
	if (!cmd_path)
		exit_handler(shell, -1);
	execve(cmd_path, cmd->cmd_av, shell->export_env);
	exit_handler(shell, errno);
}

static inline int	pre_checks(t_cmd *cmds, t_minishell *shell, int pipe_fd[2])
{
	g_shell_stats.prev_exit = 0;
	check_for_exit(cmds, shell);
	if (g_shell_stats.prev_exit)
		return (1);
	if (check_for_only_vars(cmds, shell))
		return (1);
	parse_here_docs(shell, cmds, pipe_fd);
	if (g_shell_stats.prev_exit)
		return (1);
	return (0);
}

static inline void	ft_run_cmds_fork(t_cmd *cmds, t_minishell *shell,
		int pipe_fd[2], int *cpid)
{
	*cpid = fork();
	if (*cpid == -1)
		exit_handler(shell, errno);
	if (!(*cpid))
		ft_execve(cmds, pipe_fd, shell);
	close(pipe_fd[PIPE_W]);
	if (dup2(pipe_fd[PIPE_R], STDIN_FILENO) == -1)
		exit_handler(shell, errno);
	close(pipe_fd[PIPE_R]);
}

void	ft_run_cmds(t_cmd *cmds, t_minishell *shell)
{
	int	pipe_fd[2];
	int	cpid;
	int	stdin_copy;

	if (pre_checks(cmds, shell, pipe_fd))
		return ;
	stdin_copy = dup(STDIN_FILENO);
	g_shell_stats.process_is_running = 1;
	while (cmds && g_shell_stats.process_is_running)
	{
		if (pipe(pipe_fd) == -1)
			exit_handler(shell, errno);
		if (check_for_builtins(cmds, shell, pipe_fd))
		{
			cmds = cmds->next;
			continue ;
		}
		ft_run_cmds_fork(cmds, shell, pipe_fd, &cpid);
		cmds = cmds->next;
	}
	ft_wait(cpid);
	if (dup2(stdin_copy, STDIN_FILENO) == -1 || close(stdin_copy))
		exit_handler(shell, errno);
}
