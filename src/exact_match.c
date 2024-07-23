/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exact_match.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pclaus <pclaus@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 19:31:29 by pclaus            #+#    #+#             */
/*   Updated: 2024/07/23 21:04:49 by pclaus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	exact_match(char *s, char *to_match)
{
	size_t	s_len;
	size_t	to_match_len;
	size_t	i;

	s_len = ft_strlen(s);
	to_match_len = ft_strlen(to_match);
	if (s_len != to_match_len)
		return (false);
	i = 0;
	while (s[i] && to_match[i])
	{
		if (s[i] != to_match[i])
			return (false);
		else
			i++;
	}
	return (true);
}
