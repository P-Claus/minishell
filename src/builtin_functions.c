/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pclaus <pclaus@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 19:30:07 by pclaus            #+#    #+#             */
/*   Updated: 2024/07/11 19:30:43 by pclaus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	new_pwd(t_cmd *cmd, t_minishell *shell)
{
	char	*pwd_val;

	(void)cmd;
	(void)shell;
	pwd_val = getcwd(NULL, 0);
	printf("%s\n", pwd_val);
	free(pwd_val);
	return (0);
}

int	new_echo(t_cmd *cmd, t_minishell *shell)
{
	size_t	iter;
	bool	nl;

	(void)shell;
	iter = 1;
	nl = true;
	if (cmd->cmd_av[iter] && exact_match(cmd->cmd_av[iter], "-n"))
	{
		nl = false;
		iter++;
	}
	while (cmd->cmd_av[iter])
	{
		printf("%s", cmd->cmd_av[iter++]);
		if (cmd->cmd_av[iter])
			printf(" ");
	}
	if (nl)
		printf("\n");
	return (0);
}

int	new_env(t_cmd *cmd, t_minishell *shell)
{
	size_t	i;

	(void)cmd;
	i = 0;
	while (shell->export_env[i])
		printf("%s\n", shell->export_env[i++]);
	return (0);
}

int	new_unset(t_cmd *cmd, t_minishell *shell)
{
	size_t	iter;
	t_var	*to_unset;

	iter = 1;
	while (cmd->cmd_av[iter])
	{
		to_unset = env_search_name(shell->env, cmd->cmd_av[iter]);
		env_del_target(&shell->env, to_unset);
		iter++;
	}
	env_update_export(shell);
	return (0);
}
