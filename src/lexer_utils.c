/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pclaus <pclaus@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 10:18:56 by pclaus            #+#    #+#             */
/*   Updated: 2024/07/23 14:31:40 by pclaus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	update_lexer_state(t_lexeme *lexeme, char *line, int *index)
{
	if (line[*index] == '\'' && line[*index - 1] != '=')
		lexeme->lexing_state = SQ;
	else if (line[*index] == ' ')
		lexeme->lexing_state = SP;
	else if (line[*index] == '"')
		lexeme->lexing_state = DQ;
	else if (is_meta_character(line[*index]))
		lexeme->lexing_state = META;
	else if (line[*index] == '$')
		lexeme->lexing_state = DOLLAR;
	else if (is_regular_character(line[*index]))
		lexeme->lexing_state = UNQUOTED;
}

int	is_regular_character(char c)
{
	if ((c == '!') || (c == '#') || (c == '%') || (c >= '*' && c <= ':')
		|| (c == '=') || (c == '?') || (c >= 'A' && c <= 'Z') || (c == '^')
		|| (c == '_') || (c >= 'a' && c <= 'z') || (c == '~') || (c == '{')
		|| (c == '}'))
		return (1);
	else
		return (0);
}

int	is_meta_character(char c)
{
	if (c == '|' || c == '&' || c == ';' || c == '<' || c == '>')
		return (1);
	else
		return (0);
}

void	reset_lexer_state(t_lexeme *lexeme, t_lexing_state lexing_state)
{
	add_token_to_end(&lexeme->head, create_token(lexeme->string));
	lexeme->string = NULL;
	lexeme->lexing_state = lexing_state;
}
/*
int	handle_unexpected_token(char *line, int index, char ascii_code)
{
	if ((line[index] == ascii_code) && (line[index - 1] == ascii_code)
		&& (line[index - 2] == ascii_code))
	{
		printf("syntax error near unexpected token `%c\n", line[index]);
		return (1);
	}
	else
		return (0);
}*/
