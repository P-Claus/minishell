/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_char.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pclaus <pclaus@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:15:29 by pclaus            #+#    #+#             */
/*   Updated: 2024/07/23 21:40:27 by pclaus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_strjoin_char(char **str, char c)
{
	char	*tmp;
	char	*new_str;
	size_t	i;

	if (*str == NULL)
	{
		*str = malloc(1);
		if (!*str)
			return (1);
		(*str)[0] = '\0';
	}
	new_str = malloc((ft_strlen(*str) + 2));
	if (!new_str)
		return (1);
	i = -1;
	while ((*str)[++i])
		new_str[i] = (*str)[i];
	new_str[i] = c;
	new_str[i + 1] = '\0';
	tmp = *str;
	*str = new_str;
	free(tmp);
	return (0);
}
