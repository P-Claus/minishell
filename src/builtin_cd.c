/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pclaus <pclaus@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 19:21:55 by pclaus            #+#    #+#             */
/*   Updated: 2024/07/23 21:06:43 by pclaus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	builtin_cd_no_arg(t_minishell *shell)
{
	t_var	*home_node;
	t_var	*pwd_node;

	home_node = env_search_name(shell->env, "HOME");
	if (!home_node)
	{
		printf("cd: HOME not set\n");
		exit_handler(shell, 1);
	}
	if (chdir(home_node->value))
	{
		if (errno == 2)
			printf("No such file or directory\n");
		g_shell_stats.prev_exit = 1;
		exit_handler(shell, 1);
	}
	pwd_node = env_search_name(shell->env, "PWD");
	if (pwd_node)
		env_add_var2(&shell->env, "OLDPWD", pwd_node->value, true);
	env_add_var2(&shell->env, "PWD", home_node->value, true);
	env_update_export(shell);
}

static void	builtin_cd_arg(t_cmd *cmd, t_minishell *shell)
{
	char	*pwd_value;
	t_var	*pwd_node;

	if (chdir(cmd->cmd_av[1]))
	{
		if (errno == 2)
			printf("No such file or directory\n");
		g_shell_stats.prev_exit = 1;
		exit_handler(shell, 1);
	}
	pwd_value = getcwd(NULL, 0);
	if (!pwd_value)
		exit_handler(shell, 1);
	pwd_node = env_search_name(shell->env, "PWD");
	if (pwd_node)
		env_add_var2(&shell->env, "OLDPWD", pwd_node->value, true);
	env_add_var2(&shell->env, "PWD", pwd_value, true);
	free(pwd_value);
	env_update_export(shell);
}

int	new_cd(t_cmd *cmd, t_minishell *shell)
{
	if (!cmd->cmd_av[1])
		builtin_cd_no_arg(shell);
	else
		builtin_cd_arg(cmd, shell);
	g_shell_stats.prev_exit = 0;
	return (0);
}
