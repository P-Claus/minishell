/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_execute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pclaus <pclaus@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 22:08:47 by efret             #+#    #+#             */
/*   Updated: 2024/07/23 14:29:32 by efret            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_execve(t_cmd *cmd, int pipe_fd[2], t_minishell *shell)
{
	char	*cmd_path;

	handle_sigquit_child();
	if (cmd->next && dup2(pipe_fd[PIPE_W], STDOUT_FILENO) == -1)
		old_exit_handler(1); // error
	close(pipe_fd[PIPE_W]);
	do_redirs(cmd, shell);
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
