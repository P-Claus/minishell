/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parameter_expansion_expand_string.c                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pclaus <pclaus@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 08:39:33 by pclaus            #+#    #+#             */
/*   Updated: 2024/07/23 14:52:21 by pclaus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	handle_dollar_question(t_string_info *s_info)
{
	s_info->env_value = ft_itoa((int)g_shell_stats.prev_exit);
	s_info->free_env_value = true;
}

static void	handle_other_variables(t_string_info *s_info, t_minishell *shell)
{
	s_info->env_value = get_env_value(shell->env, s_info->trimmed_parameter + 1,
			false, shell);
	if (s_info->env_value == NULL)
	{
		s_info->env_value = strdup("");
		s_info->free_env_value = true;
	}
	else
		s_info->free_env_value = false;
}

void	expand_env_variable(char **string, t_minishell *shell,
		t_string_info *s_info, int iter)
{
	calculate_start_and_end(string, &s_info->start, &s_info->end, iter);
	s_info->start += iter;
	s_info->end += iter;
	s_info->trimmed_parameter = get_trimmed_parameter(s_info->start,
			s_info->end, string, shell);
	if (exact_match(s_info->trimmed_parameter, "$?"))
		handle_dollar_question(s_info);
	else
		handle_other_variables(s_info, shell);
	s_info->expanded_string = get_expanded_string(s_info, string, shell);
	free(*string);
	*string = s_info->expanded_string;
	free(s_info->trimmed_parameter);
	s_info->trimmed_parameter = NULL;
	if (s_info->free_env_value == true)
		free(s_info->env_value);
}

void	expand_string(char **string, t_minishell *shell, t_string_info *s_info,
		int iter)
{
	if (((*string) + iter)[0] == '$')
	{
		if (((*string + iter)[1]) == ' ' || ((*string) + iter)[1] == '"')
			return ;
		else
			expand_env_variable(string, shell, s_info, iter);
	}
	else
		return ;
}
