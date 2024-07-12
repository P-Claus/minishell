/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handle_quotes.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pclaus <pclaus@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 19:44:14 by pclaus            #+#    #+#             */
/*   Updated: 2024/07/11 19:45:08 by pclaus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_single_quotes(t_lexeme *lexeme, char *line, int *index)
{
	while (line[*index] != '\0' && lexeme->lexing_state == SQ)
	{
		ft_strjoin_char(&lexeme->string, (line[*index]));
		(*index)++;
		if (line[*index] == '\'')
		{
			ft_strjoin_char(&lexeme->string, (line[*index]));
			reset_lexer_state(lexeme, START);
			(*index)++;
		}
	}
}

void	handle_double_quotes(t_lexeme *lexeme, char *line, int *index)
{
	while (line[*index] != '\0' && lexeme->lexing_state == DQ)
	{
		ft_strjoin_char(&lexeme->string, (line[*index]));
		(*index)++;
		if (line[*index] == '"')
		{
			ft_strjoin_char(&lexeme->string, (line[*index]));
			reset_lexer_state(lexeme, START);
			(*index)++;
		}
	}
}
