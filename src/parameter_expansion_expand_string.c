/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parameter_expansion_expand_string.c                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pclaus <pclaus@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 08:39:33 by pclaus            #+#    #+#             */
/*   Updated: 2024/07/17 09:02:14 by pclaus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	init_s_info(t_string_info *s_info)
{
	s_info->start = 0;
	s_info->end = 0;
	s_info->free_env_value = false;
	s_info->trimmed_parameter = NULL;
	s_info->env_value = NULL;
	s_info->expanded_string = NULL;
}

void	expand_string(char **string, t_minishell *shell)
{
	t_string_info	s_info;

	init_s_info(&s_info);
	while (ft_strchr(*string, '$'))
	{
		calculate_start_and_end(string, &s_info.start, &s_info.end);
		s_info.trimmed_parameter = get_trimmed_parameter(s_info.start,
				s_info.end, string);
		if (exact_match(s_info.trimmed_parameter, "$?"))
		{
			s_info.env_value = ft_itoa((int)g_shell_stats.prev_exit);
			s_info.free_env_value = true;
		}
		else
			s_info.env_value = get_env_value(shell->env,
					s_info.trimmed_parameter + 1);
		s_info.expanded_string = get_expanded_string(s_info.start, string,
				s_info.env_value, s_info.trimmed_parameter);
		free(*string);
		*string = s_info.expanded_string;
		free(s_info.trimmed_parameter);
		if (s_info.free_env_value == true)
			free(s_info.env_value);
	}
}
