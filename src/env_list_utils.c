/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efret <efret@student.19.be>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 13:39:36 by efret             #+#    #+#             */
/*   Updated: 2024/07/20 16:23:57 by efret            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_env(t_var **env)
{
	if (!env || !*env)
		return ;
	while (*env)
		env_del_target(env, *env);
}

t_var	*create_env_var(char *name, char *val, bool is_exp)
{
	t_var	*var;

	var = malloc(sizeof(t_var));
	if (var)
		*var = (t_var){name, val, is_exp, NULL, NULL};
	return (var);
}

int	env_add_back(t_var **head, t_var *new_node)
{
	t_var	*iter;

	if (!head)
		return (errno = EINVAL, 1);
	if (!(*head))
		return (*head = new_node, 0);
	iter = *head;
	while (iter->next)
		iter = iter->next;
	iter->next = new_node;
	new_node->prev = iter;
	return (0);
}

void	env_del_target(t_var **head, t_var *node)
{
	t_var	*next;
	t_var	*prev;

	if (!node || !head)
		return ;
	prev = node->prev;
	next = node->next;
	if (next)
		next->prev = prev;
	if (prev)
		prev->next = next;
	else
		*head = next;
	free(node->name);
	if (node->value)
		free(node->value);
	free(node);
}

t_var	*env_search_name(t_var *head, char *name)
{
	t_var	*iter;

	if (!name)
		return (NULL);
	iter = head;
	while (iter && !exact_match(iter->name, name))
		iter = iter->next;
	return (iter);
}
