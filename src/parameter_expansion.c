/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parameter_expansion.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pclaus <pclaus@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 17:27:03 by pclaus            #+#    #+#             */
/*   Updated: 2024/07/23 14:16:02 by pclaus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	init_s_info(t_string_info *s_info)
{
	s_info->start = 0;
	s_info->end = 0;
	s_info->free_env_value = false;
	s_info->trimmed_parameter = NULL;
	s_info->env_value = NULL;
	s_info->expanded_string = NULL;
}

void	process_token(char **string, t_minishell *shell)
{
	int				iter;
	t_string_info	s_info;

	init_s_info(&s_info);
	iter = 0;
	while (iter < (int)ft_strlen(*string) && (*string)[iter]
		&& (*string)[iter] != '\0')
	{
		if (ft_strlen(*string) == 1 && (*string)[iter] == '$')
			return ;
		else if ((*string)[iter] == '"' || (*string)[iter] == '$')
			expand_string(string, shell, &s_info, iter);
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
