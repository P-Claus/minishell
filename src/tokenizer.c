/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pclaus <pclaus@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 15:37:56 by pclaus            #+#    #+#             */
/*   Updated: 2024/07/23 14:16:55 by pclaus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*create_token(char *string)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->str = string;
	new_token->tag = CMD;
	new_token->next = NULL;
	return (new_token);
}

void	add_token_to_end(t_token **head, t_token *new_token)
{
	t_token	*iter;

	if (*head == NULL)
	{
		*head = new_token;
		return ;
	}
	iter = *head;
	while (iter->next != NULL)
		iter = iter->next;
	iter->next = new_token;
}

void	free_tokens(t_token **tokens)
{
	t_token	*free_me;

	if (!tokens || !(*tokens))
		return ;
	free_me = *tokens;
	while (*tokens)
	{
		*tokens = (*tokens)->next;
		free(free_me->str);
		free(free_me);
		free_me = (*tokens);
	}
	*tokens = NULL;
}
