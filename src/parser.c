/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pclaus <pclaus@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 19:09:58 by pclaus            #+#    #+#             */
/*   Updated: 2024/07/22 17:00:25 by efret            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	determine_quote_type(t_token *token)
{
	if (token->str[0] == '\'' && token->str[ft_strlen(token->str) - 1] == '\'')
		token->tag = SINGLE_Q;
	else if (token->str[0] == '"' && token->str[ft_strlen(token->str)
			- 1] == '"')
		token->tag = DOUBLE_Q;
}

void	determine_token_type(t_token *token, int iter)
{
	if ((token->str[0] == '\'' && token->str[ft_strlen(token->str) - 1] == '\'')
		|| (token->str[0] == '"' && token->str[ft_strlen(token->str)
				- 1] == '"'))
		determine_quote_type(token);
	else if ((ft_strchr(token->str, '=') && ft_strchr(token->str, '\'')))
	{
		while (token->str[iter++])
		{
			if (token->str[iter] == '=' && token->str[iter + 1] == '\'')
				token->tag = MAKE_VAR_SINGLE;
		}
	}
	else if ((ft_strchr(token->str, '=') && ft_strchr(token->str, '"')))
	{
		while (token->str[iter++])
		{
			if (token->str[iter] == '=' && token->str[iter + 1] == '"')
				token->tag = MAKE_VAR_DOUBLE;
		}
	}
	else if (ft_strchr(token->str, '=') && ft_strlen(token->str) > 1)
		token->tag = MAKE_VAR;
	else
		token->tag = CMD;
}

void	parser(t_token **token)
{
	t_token	*iter;

	if (!token)
		return ;
	iter = *token;
	while (iter && iter->str != NULL)
	{
		if ((ft_strnstr(iter->str, "|", 1) != NULL))
			iter->tag = PIPE;
		else if ((ft_strnstr(iter->str, "<", 1) != NULL)
			|| (ft_strnstr(iter->str, ">", 1) != NULL)
			|| (ft_strnstr(iter->str, "<<", 2) != NULL)
			|| (ft_strnstr(iter->str, ">>", 2) != NULL))
			iter->tag = REDIRECT;
		else
			determine_token_type(iter, 0);
		iter = iter->next;
	}
}
