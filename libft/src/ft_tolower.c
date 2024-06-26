/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efret <efret@student.19.be>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 14:50:20 by efret             #+#    #+#             */
/*   Updated: 2024/06/18 14:16:20 by efret            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/libft.h"

int	ft_tolower(int c)
{
	if (c < 0)
		return (c);
	c = (unsigned char)c;
	if ('A' <= c && c <= 'Z')
		return (c - 'A' + 'a');
	return (c);
}
