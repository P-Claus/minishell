/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handle_make_var.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pclaus <pclaus@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 19:45:54 by pclaus            #+#    #+#             */
/*   Updated: 2024/07/11 19:46:36 by pclaus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_var_single(t_lexeme *lexeme, char *line, int **index)
{
	lexeme->lexing_state = VAR_SINGLE;
	ft_strjoin_char(&lexeme->string, (line[**index]));
	while (line[**index] != '\0' && lexeme->lexing_state == VAR_SINGLE)
	{
		(**index)++;
		ft_strjoin_char(&lexeme->string, (line[**index]));
		if (line[**index] == '\'')
		{
			reset_lexer_state(lexeme, START);
			(**index)++;
		}
	}
}

void	handle_var_double(t_lexeme *lexeme, char *line, int **index)
{
	lexeme->lexing_state = VAR_DOUBLE;
	ft_strjoin_char(&lexeme->string, (line[**index]));
	while (line[**index] != '\0' && lexeme->lexing_state == VAR_DOUBLE)
	{
		(**index)++;
		ft_strjoin_char(&lexeme->string, (line[**index]));
		if (line[**index] == '"')
		{
			reset_lexer_state(lexeme, START);
			(**index)++;
		}
	}
}

void	handle_var_make(t_lexeme *lexeme, char *line, int **index)
{
	if (line[**index] == '\'' && line[**index - 1] == '=')
		handle_var_single(lexeme, line, index);
	else if (line[**index] == '"' && line[**index - 1] == '=')
		handle_var_double(lexeme, line, index);
}
