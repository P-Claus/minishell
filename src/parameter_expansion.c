/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parameter_expansion.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pclaus <pclaus@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 17:27:03 by pclaus            #+#    #+#             */
/*   Updated: 2024/07/11 18:35:10 by pclaus           ###   ########.fr       */
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

static char	*get_env_value(t_var *env, char *name)
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

	while (ft_strchr(*string, '$'))
	{
		calculate_start_and_end(string, &start, &end);
		trimmed_parameter = get_trimmed_parameter(start, end, string);
		printf("The trim is: %s\n", trimmed_parameter);
		if (exact_match(trimmed_parameter, "$?"))
			env_value = ft_itoa((int)g_shell_stats.prev_exit);
		else
			env_value = get_env_value(shell->env, trimmed_parameter + 1);
		expanded_string = get_expanded_string(start, string, env_value,
				trimmed_parameter);
		free(*string);
		*string = expanded_string;
		free(trimmed_parameter);
	}
}

void	process_token(char **string, t_minishell *shell)
{
	int	iter;

	iter = 0;
	while ((*string)[iter] && (*string)[iter] != '\0')
	{
		if ((*string)[iter] == '"' || (*string)[iter] == '$')
			expand_string(string, shell);
		iter++;
	}
}

void	expand_parameters(t_token **token, t_minishell *shell)
{
	t_token	*iter;

	iter = *token;
	while (iter)
	{
		if (!(iter->tag == SINGLE_Q))
			process_token(&iter->str, shell);
		if (iter->next)
			iter = iter->next;
		else
			break ;
	}
}
