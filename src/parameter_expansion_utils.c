/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parameter_expansion_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pclaus <pclaus@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 08:14:42 by pclaus            #+#    #+#             */
/*   Updated: 2024/07/23 15:03:02 by pclaus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	calculate_start_and_end(char **string, int *start, int *end, int iter)
{
	int	i;

	i = 0;
	while (((*string) + iter)[i] != '\0')
	{
		if (((*string) + iter)[i] == '$' && ((*string) + iter)[i + 1] == '?')
		{
			*start = i;
			*end = i + 2;
			return ;
		}
		if (((*string) + iter)[i] == '$')
		{
			*start = i;
			while (ft_isalnum(((*string) + iter)[i]) || ((*string)
					+ iter)[i] == '_' || ((*string) + iter)[i] == '$'
				|| ((*string) + iter)[i] == '{' || ((*string) + iter)[i] == '}')
				i++;
			*end = i;
			return ;
		}
		i++;
	}
}

char	*get_trimmed_parameter(int start, int end, char **string,
		t_minishell *shell)
{
	char	*trimmed_parameter;

	trimmed_parameter = malloc((end - start + 1) * sizeof(char));
	if (!trimmed_parameter)
		exit_handler(shell, errno);
	ft_strlcpy(trimmed_parameter, *string + start, end - start + 1);
	return (trimmed_parameter);
}

char	*get_expanded_string(t_string_info *s_info, char **string,
		t_minishell *shell)
{
	char	*string_to_expand;
	char	*expanded_string;
	int		total_len;

	string_to_expand = ft_strdup(s_info->env_value);
	total_len = ft_strlen(string_to_expand) + ft_strlen(*string)
		- ft_strlen(s_info->trimmed_parameter);
	expanded_string = malloc((total_len + 1) * sizeof(char));
	if (!expanded_string)
		exit_handler(shell, errno);
	ft_strlcpy(expanded_string, *string, s_info->start + 1);
	ft_strlcat(expanded_string, string_to_expand, total_len + 1);
	ft_strlcat(expanded_string, *string + s_info->start
		+ ft_strlen(s_info->trimmed_parameter), total_len + 1);
	free(string_to_expand);
	return (expanded_string);
}

char	*get_env_value(t_var *env, char *name, bool has_braces,
		t_minishell *shell)
{
	t_var	*node;
	char	*trimmed_name;
	char	*double_trim;

	trimmed_name = name;
	double_trim = NULL;
	if (ft_strchr(name, '{') && ft_strchr(name, '}'))
	{
		trimmed_name = ft_strtrim(name, "{");
		if (!trimmed_name)
			exit_handler(shell, errno);
		double_trim = ft_strtrim(trimmed_name, "}");
		free(trimmed_name);
		if (!double_trim)
			exit_handler(shell, errno);
		trimmed_name = double_trim;
		has_braces = true;
	}
	node = env_search_name(env, trimmed_name);
	if (has_braces)
		free(trimmed_name);
	if (node && node->value)
		return (node->value);
	return ("");
}
