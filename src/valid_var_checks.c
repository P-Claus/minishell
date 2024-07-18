/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_var_checks.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efret <efret@student.19.be>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 14:54:01 by efret             #+#    #+#             */
/*   Updated: 2024/07/18 16:39:32 by efret            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static inline bool	is_valid_name_char(char c)
{
	if (c == '_' || ft_isalnum(c))
		return (true);
	return (false);
}

bool	valid_var_name(char *s)
{
	if (!s || !(*s) || ft_isdigit(*s))
		return (false);
	while (*s && *s != '=')
		if (!is_valid_name_char(*s++))
			return (false);
	return (true);
}

bool	valid_var_token(char *s)
{
	if (!s || !(*s))
		return (false);
	if (ft_strchr(s, '=') && valid_var_name(s))
		return (true);
	return (false);
}
