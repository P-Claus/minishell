/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handler_functions.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pclaus <pclaus@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 10:20:46 by pclaus            #+#    #+#             */
/*   Updated: 2024/07/04 16:30:07 by pclaus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_single_quotes(t_lexeme *lexeme, char *line, int *index)
{
	(*index)++;
	while (line[*index] != '\0' && lexeme->lexing_state == SQ)
	{
		ft_strjoin_char(&lexeme->string, (line[*index]));
		(*index)++;
		if (line[*index] == '\'')
		{
			reset_lexer_state(lexeme, START);
			(*index)++;
		}
	}
}

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

void	handle_unquoted(t_lexeme *lexeme, char *line, int *index)
{
	while (line[*index] != '\0' && lexeme->lexing_state == UNQUOTED)
	{
		ft_strjoin_char(&lexeme->string, (line[*index]));
		(*index)++;
		if (line[*index] == '\'' && line[*index - 1] == '=')
		{
			handle_var_single(lexeme, line, &index);
			break ;
		}
		if (is_meta_character(line[*index]))
		{

			reset_lexer_state(lexeme, START);
			break ;
		}
		if (!is_regular_character(line[*index]) && (line[*index] != '_' || line[*index] != '\0') && (line[*index] != '$'))
		{
			reset_lexer_state(lexeme, START);
			if (line[*index] != '\0')
				(*index)++;
		}
	}
}

void	handle_space(t_lexeme *lexeme, char *line, int *index)
{
	while (line[*index] != '\0' && lexeme->lexing_state == SP)
	{
		(*index)++;
		if (line[*index] != ' ')
			lexeme->lexing_state = START;
	}
}

void	handle_double_quotes(t_lexeme *lexeme, char *line, int *index)
{
	(*index)++;
	while (line[*index] != '\0' && lexeme->lexing_state == DQ)
	{
		ft_strjoin_char(&lexeme->string, (line[*index]));
		(*index)++;
		if (line[*index] == '"')
		{
			reset_lexer_state(lexeme, START);
			(*index)++;
		}
	}
}

void	handle_dollar(t_lexeme *lexeme, char *line, int *index)
{
	bool	inside_curly_brackets;

	inside_curly_brackets = false;
	while (lexeme->lexing_state == DOLLAR)
	{
		if (line[*index] == '{')
			inside_curly_brackets = true;
		if (line[*index] == '}')
		{
			ft_strjoin_char(&lexeme->string, (line[*index]));
			(*index)++;
			reset_lexer_state(lexeme, START);
			break ;
		}
		if ((line[*index] != ' ' && line[*index] != '\0')
			|| (line[*index] == ' ' && inside_curly_brackets == true))
		{
			ft_strjoin_char(&lexeme->string, (line[*index]));
			(*index)++;
		}
		else
			reset_lexer_state(lexeme, START);
	}
}
