/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pclaus <pclaus@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 13:56:40 by pclaus            #+#    #+#             */
/*   Updated: 2024/07/11 15:12:02 by efret            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*lexer(char *line, t_minishell *shell)
{
	t_lexeme	lexeme;
	int	index;

	index = 0;
	lexeme.lexing_state = START;
	lexeme.string = NULL;
	lexeme.head = NULL;
	while (line && line[index])
	{
		update_lexer_state(&lexeme, line, &index);
		if (lexeme.lexing_state == UNQUOTED)
			handle_unquoted(&lexeme, line, &index);
		else if (lexeme.lexing_state == SQ)
			handle_single_quotes(&lexeme, line, &index);
		else if (lexeme.lexing_state == DQ)
			handle_double_quotes(&lexeme, line, &index);
		else if (lexeme.lexing_state == META)
			handle_meta_char(&lexeme, line, &index);
		else if (lexeme.lexing_state == SP)
			handle_space(&lexeme, line, &index);
		else if (lexeme.lexing_state == DOLLAR)
			handle_dollar(&lexeme, line, &index);
		else if (lexeme.lexing_state == START)
			break ;
		else
			old_exit_handler(1);
	}
	parser(&lexeme.head);
	expand_parameters(&lexeme.head, shell);
	remove_quotes_from_variables(&lexeme.head);
	remove_quotes_from_quoted_string(&lexeme.head);
//	free_list(&lexeme.head);
//	print_list(&lexeme.head);
	return (lexeme.head);
}
