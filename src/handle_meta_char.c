/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_meta_char.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pclaus <pclaus@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 21:31:35 by pclaus            #+#    #+#             */
/*   Updated: 2024/07/11 19:36:06 by pclaus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_meta_char(t_lexeme *lexeme, char *line, int *index)
{
	int	iter;

	iter = 0;
	while (lexeme->lexing_state == META)
	{
		ft_strjoin_char(&lexeme->string, (line[*index]));
		(*index)++;
		iter++;
		if (!is_meta_character(line[*index]) || (line[*index] != line[*index
					- 1]) || iter == 2 || line[*index] == '\0')
		{
			reset_lexer_state(lexeme, START);
			break ;
		}
	}
}
