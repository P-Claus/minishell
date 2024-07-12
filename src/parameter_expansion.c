/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parameter_expansion.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pclaus <pclaus@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 17:27:03 by pclaus            #+#    #+#             */
/*   Updated: 2024/07/12 08:25:11 by pclaus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	process_token(char **string, t_minishell *shell)
{
	int	iter;

	iter = 0;
	while ((*string)[iter] && (*string)[iter] != '\0')
	{
		if ((*string)[iter] == '"' || (*string)[iter] == '$')
			expand_string(string, shell);
		iter++;
	}
}

void	expand_parameters(t_token **token, t_minishell *shell)
{
	t_token	*iter;

	iter = *token;
	while (iter)
	{
		if (!(iter->tag == SINGLE_Q))
			process_token(&iter->str, shell);
		if (iter->next)
			iter = iter->next;
		else
			break ;
	}
}
