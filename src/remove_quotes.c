/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pclaus <pclaus@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 09:23:16 by pclaus            #+#    #+#             */
/*   Updated: 2024/07/12 08:59:08 by pclaus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	remove_quotes_from_quoted_string(t_token **token)
{
	t_token	*iter;
	char	*trimmed_string;

	iter = *token;
	while (iter)
	{
		if (iter->str[0] == '\'' && iter->str[ft_strlen(iter->str) - 1] == '\'')
		{
			trimmed_string = ft_strtrim(iter->str, "'");
			free(iter->str);
			iter->str = trimmed_string;
		}
		else if (iter->str[0] == '"' && iter->str[ft_strlen(iter->str) - 1])
		{
			trimmed_string = ft_strtrim(iter->str, "\"");
			free(iter->str);
			iter->str = trimmed_string;
		}
		if (iter->next)
			iter = iter->next;
		else
			break ;
	}
}

static void	process_tokens_to_remove_quotes(char **string)
{
	int		i;
	int		len;
	char	*trimmed_string;

	i = 0;
	while ((*string)[i] != '\0')
	{
		if ((*string)[i] == '=' && ((*string)[i + 1] == '\'' || (*string)[i
				+ 1] == '"'))
		{
			len = ft_strlen(*string) - 2;
			trimmed_string = malloc(len * sizeof(char));
			if (!trimmed_string)
				return ;//malloc failure
			ft_memcpy(trimmed_string, *string, i + 1);
			ft_memcpy(trimmed_string + ft_strlen(trimmed_string), (*string) + i
				+ 2, ft_strlen(*string) + i - 3);
			free(*string);
			*string = trimmed_string;
		}
		i++;
	}
}

void	remove_quotes_from_variables(t_token **token)
{
	t_token	*iter;

	iter = *token;
	while (iter)
	{
		if (ft_strchr(iter->str, '\'') || ft_strchr(iter->str, '"'))
			process_tokens_to_remove_quotes(&iter->str);
		if (iter->next)
			iter = iter->next;
		else
			break ;
	}
}
