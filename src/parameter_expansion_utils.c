/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parameter_expansion_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pclaus <pclaus@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 08:14:42 by pclaus            #+#    #+#             */
/*   Updated: 2024/07/16 08:48:06 by pclaus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	calculate_start_and_end(char **string, int *start, int *end)
{
	int	i;

	i = 0;
	while ((*string)[i] != '\0')
	{
		if ((*string)[i] == '$' && (*string)[i + 1] == '?')
		{
			*start = i;
			*end = i + 2;
			return ;
		}
		else if ((*string)[i] == '$')
		{
			*start = i;
			while (ft_isalnum((*string)[i]) || (*string)[i] == '_'
				|| (*string)[i] == '$' || (*string)[i] == '{'
				|| (*string)[i] == '}')
				i++;
			*end = i;
			return ;
		}
		i++;
	}
}

char	*get_trimmed_parameter(int start, int end, char **string)
{
	char	*trimmed_parameter;

	trimmed_parameter = malloc((end - start + 1) * sizeof(char));
	if (!trimmed_parameter)
		return (NULL); //malloc failure
	ft_strlcpy(trimmed_parameter, *string + start, end - start + 1);
	return (trimmed_parameter);
}

char	*get_expanded_string(int start, char **string, char *env_value,
		char *trimmed_parameter)
{
	char	*string_to_expand;
	char	*expanded_string;
	int		total_len;

	string_to_expand = ft_strdup(env_value);
	total_len = ft_strlen(string_to_expand) + ft_strlen(*string)
		- ft_strlen(trimmed_parameter);
	expanded_string = malloc((total_len + 1) * sizeof(char *));
	if (!expanded_string)
		return (NULL); //malloc failure
	ft_strlcpy(expanded_string, *string, start + 1);
	ft_strlcat(expanded_string, string_to_expand, total_len + 1);
	ft_strlcat(expanded_string, *string + start + ft_strlen(trimmed_parameter),
		total_len + 1);
	free(string_to_expand);
	return (expanded_string);
}

char	*get_env_value(t_var *env, char *name)
{
	t_var	*node;
	char	*trimmed_name;

	trimmed_name = name;
	if (ft_strchr(name, '{') && ft_strchr(name, '}'))
	{
		trimmed_name = ft_strtrim(name, "{");
		trimmed_name = ft_strtrim(trimmed_name, "}");
	}
	node = env_search_name(env, trimmed_name);
	if (node && node->value)
		return (node->value);
	return ("");
}

void	expand_string(char **string, t_minishell *shell)
{
	char	*trimmed_parameter;
	char	*expanded_string;
	int		start;
	int		end;
	char	*env_value;
	bool	free_env_value;

	while (ft_strchr(*string, '$'))
	{
		calculate_start_and_end(string, &start, &end);
		trimmed_parameter = get_trimmed_parameter(start, end, string);
		if (exact_match(trimmed_parameter, "$?"))
		{
			env_value = ft_itoa((int)g_shell_stats.prev_exit);
			free_env_value = true;
		}
		else
			env_value = get_env_value(shell->env, trimmed_parameter + 1);
		expanded_string = get_expanded_string(start, string, env_value,
				trimmed_parameter);
		free(*string);
		*string = expanded_string;
		free(trimmed_parameter);
		if (free_env_value == true)
			free(env_value);
	}
}
