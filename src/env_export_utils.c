/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_export_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efret <efret@student.19.be>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 17:38:48 by efret             #+#    #+#             */
/*   Updated: 2024/07/11 15:05:00 by efret            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_env_export(char ***exp)
{
	size_t	i;

	if (!exp || !(*exp))
		return ;
	i = 0;
	while ((*exp)[i])
		free((*exp)[i++]);
	free((*exp));
	*exp = NULL;
}

static size_t	count_export_vars(t_var *env_list)
{
	size_t	ret;

	ret = 0;
	while (env_list)
	{
		if (env_list->is_exp && env_list->value)
			ret++;
		env_list = env_list->next;
	}
	return (ret);
}

static void	envp_add_var(char **ret_var, t_var *list_var)
{
	char	*tmp;
	char	*value;

	if (!list_var->value)
		return ;
	tmp = ft_strdup(list_var->name);
	if (!tmp)
		old_exit_handler(1); //Error handling
	if (ft_strjoin_char(&tmp, '='))
		old_exit_handler(1); //Error handling
	value = ft_strjoin(tmp, list_var->value);
	if (!value)
		(free(tmp), old_exit_handler(1));
	*ret_var = value;
}

char	**make_export_envp(t_var *env_list)
{
	size_t	i;
	size_t	count_vars;
	char	**ret;

	count_vars = count_export_vars(env_list);
	ret = malloc(sizeof(char *) * (count_vars + 1));
	if (!ret)
		return (NULL);
	i = 0;
	while (env_list && i < count_vars)
	{
		if (env_list->is_exp && env_list->value)
			envp_add_var(&ret[i++], env_list);
		env_list = env_list->next;
	}
	ret[count_vars] = NULL;
	if (i != count_vars)
		old_exit_handler(1); // Some oopsi happen
	return (ret);
}

void	env_update_export(t_minishell *shell)
{
	free_env_export(&shell->export_env);
	shell->export_env = make_export_envp(shell->env);
}
