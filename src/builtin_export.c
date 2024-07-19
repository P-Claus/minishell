/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pclaus <pclaus@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 19:28:14 by pclaus            #+#    #+#             */
/*   Updated: 2024/07/18 15:20:02 by efret            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Technically should print them sorted */
static int	export_print(t_minishell *shell)
{
	t_var	*var;

	var = shell->env;
	while (var)
	{
		if (var->is_exp)
		{
			printf("declare -x %s", var->name);
			if (var->value)
				printf("=\"%s\"", var->value);
			printf("\n");
		}
		var = var->next;
	}
	return (0);
}

int	new_export(t_cmd *cmd, t_minishell *shell)
{
	size_t	iter;
	t_var	*var;

	iter = 1;
	if (!cmd->cmd_av[iter])
		return (export_print(shell));
	while (cmd->cmd_av[iter])
	{
		if (!valid_var_name(cmd->cmd_av[iter]))
		{
			g_shell_stats.prev_exit = 1;
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(cmd->cmd_av[iter++], STDERR_FILENO);
			ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
			continue ;
		}
		if (ft_strchr(cmd->cmd_av[iter], '='))
			var = env_add_var(&shell->env, cmd->cmd_av[iter], true);
		else
			var = env_add_var_only(&shell->env, cmd->cmd_av[iter], true);
		if (!var)
			break ;
		iter++;
	}
	env_update_export(shell);
	return (g_shell_stats.prev_exit);
}
