/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efret <efret@student.19.be>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 12:01:35 by efret             #+#    #+#             */
/*   Updated: 2024/07/11 15:01:48 by efret            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_minishell(t_minishell *shell)
{
	free_env(&shell->env);
	free_env_export(&shell->export_env);
}

void	exit_handler(t_minishell *shell, int status)
{
	rl_clear_history();
	if (!status)
		write(2, "exit\n", 5);
	free_minishell(shell);
	exit(g_shell_stats.prev_exit);
}

void	old_exit_handler(int status)
{
	rl_clear_history();
	if (!status)
		write(2, "exit\n", 5);
	exit(status);
}
