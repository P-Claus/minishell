/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_special_checks.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efret <efret@student.19.be>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 19:15:17 by efret             #+#    #+#             */
/*   Updated: 2024/07/24 15:15:35 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	update_cmd_av(t_cmd *cmd, t_minishell *shell)
{
	size_t	i;
	char	**old_cmd_av;

	old_cmd_av = cmd->cmd_av;
	i = 0;
	while (old_cmd_av[i])
		i++;
	cmd->cmd_av = malloc(sizeof(char *) * i);
	if (!cmd->cmd_av)
		exit_handler(shell, -1);
	cmd->cmd_av[i - 1] = NULL;
	i = 0;
	while (old_cmd_av[++i])
		cmd->cmd_av[i - 1] = old_cmd_av[i];
	free(old_cmd_av);
}

void	check_for_leading_vars(t_cmd *cmd, t_minishell *shell)
{
	while (valid_var_token(cmd->cmd_av[0]))
	{
		if (!env_add_var(&shell->env, cmd->cmd_av[0], true))
			exit_handler(shell, 1);
		update_cmd_av(cmd, shell);
		env_update_export(shell);
	}
}

int	check_for_only_vars(t_cmd *cmds, t_minishell *shell)
{
	size_t	i;
	t_var	*node;

	i = 0;
	if (!(cmds) || (cmds && cmds->next))
		return (0);
	while (cmds->cmd_av[i])
		if (!valid_var_token(cmds->cmd_av[i++]))
			return (0);
	i = 0;
	while (cmds->cmd_av[i])
	{
		node = env_add_var(&shell->env, cmds->cmd_av[i], false);
		if (node && node->is_exp)
			env_update_export(shell);
		i++;
	}
	return (1);
}
