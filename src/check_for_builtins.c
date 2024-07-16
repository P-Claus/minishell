/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_for_builtins.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pclaus <pclaus@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 19:40:59 by pclaus            #+#    #+#             */
/*   Updated: 2024/07/13 12:31:25 by efret            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	builtin_wrapper(int (*func)(t_cmd *, t_minishell *), t_cmd *cmd,
		t_minishell *shell, int pipe_fd[2])
{
	int	stdout_copy;

	stdout_copy = dup(STDOUT_FILENO);
	if (cmd->next && dup2(pipe_fd[PIPE_W], STDOUT_FILENO) == -1)
		old_exit_handler(1); //error
	do_redirs(cmd);
	g_shell_stats.prev_exit = func(cmd, shell);
	close(pipe_fd[PIPE_W]);
	if (dup2(pipe_fd[PIPE_R], STDIN_FILENO) == -1 || close(pipe_fd[PIPE_R]))
		old_exit_handler(1);
	if (dup2(stdout_copy, STDOUT_FILENO) == -1 || close(stdout_copy))
		old_exit_handler(1);
}

int	check_for_builtins(t_cmd *cmd, t_minishell *shell, int pipe_fd[2])
{
	(void)pipe_fd;
	if (exact_match(cmd->cmd_av[0], "echo"))
		builtin_wrapper(new_echo, cmd, shell, pipe_fd);
	else if (exact_match(cmd->cmd_av[0], "cd"))
		builtin_wrapper(new_cd, cmd, shell, pipe_fd);
	else if (exact_match(cmd->cmd_av[0], "pwd"))
		builtin_wrapper(new_pwd, cmd, shell, pipe_fd);
	else if (exact_match(cmd->cmd_av[0], "export"))
		builtin_wrapper(new_export, cmd, shell, pipe_fd);
	else if (exact_match(cmd->cmd_av[0], "unset"))
		builtin_wrapper(new_unset, cmd, shell, pipe_fd);
	else if (exact_match(cmd->cmd_av[0], "env"))
		builtin_wrapper(new_env, cmd, shell, pipe_fd);
	else if (exact_match(cmd->cmd_av[0], "exit"))
		;
	else
		return (0);
	return (1);
}
