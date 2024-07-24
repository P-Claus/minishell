/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_make_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efret <efret@student.19.be>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 14:36:12 by efret             #+#    #+#             */
/*   Updated: 2024/07/24 15:12:11 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_var	*env_add_var_only(t_var **head, char *name, bool is_exp)
{
	char	*var_value;
	char	*var_name;
	t_var	*node;

	node = env_search_name(*head, name);
	if (node)
	{
		node->is_exp = is_exp;
		return (node);
	}
	var_name = ft_strdup(name);
	if (!var_name)
		return (NULL);
	var_value = NULL;
	node = create_env_var(var_name, var_value, is_exp);
	if (!node || env_add_back(head, node))
		return (free(var_name), NULL);
	return (node);
}

t_var	*env_add_var2(t_var **head, char *name, char *value, bool exp)
{
	char	*var_name;
	char	*var_value;
	t_var	*node;

	if (!name)
		return (errno = EINVAL, NULL);
	var_name = ft_strdup(name);
	if (!var_name)
		return (NULL);
	var_value = ft_strdup(value);
	if (!var_value && value)
		return (free(var_name), NULL);
	node = env_search_name(*head, var_name);
	if (node)
	{
		if (node->value)
			free(node->value);
		node->value = var_value;
		node->is_exp = (node->is_exp == true) + (node->is_exp == false) * exp;
		return (free(var_name), node);
	}
	node = create_env_var(var_name, var_value, exp);
	if (!node || env_add_back(head, node))
		return (free(var_name), free(var_value), NULL);
	return (node);
}

t_var	*env_add_var(t_var **head, char *token, bool is_exp)
{
	char	*name;
	char	*value;
	char	*equal_sign;
	t_var	*node;

	equal_sign = ft_strchr(token, '=');
	if (!equal_sign)
		return (NULL);
	name = ft_substr(token, 0, equal_sign - token);
	if (!name)
		return (NULL);
	value = ft_strdup(&equal_sign[1]);
	if (!value)
		return (free(name), NULL);
	node = env_add_var2(head, name, value, is_exp);
	free(name);
	free(value);
	return (node);
}

void	env_load(t_minishell *shell, char **envp)
{
	while (*envp)
	{
		env_add_var(&shell->env, *envp, true);
		if (errno)
			exit_handler(shell, errno);
		envp++;
	}
}
