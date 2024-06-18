/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efret <efret@student.19.be>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 16:02:06 by efret             #+#    #+#             */
/*   Updated: 2024/06/07 17:36:07 by efret            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	**unpack_path(t_var *path_var)
{
	char	**paths;

	if (!path_var)
		return (errno = EINVAL, NULL);
	paths = ft_split(path_var->value, ':');
	return (paths);
}

static char	*check_abs_rel(char *cmd_name)
{
	char	*cmd_path;

	if (!access(cmd_name, X_OK))
	{
		cmd_path = ft_strdup(cmd_name);
		if (!cmd_path)
			return (NULL);
		return (cmd_path);
	}
	if (cmd_name[0] == '/')
		return (errno = ENOENT, NULL);
	return (errno = 0, NULL);
}

static char	*check_paths(char *cmd_name, char **paths)
{
	size_t	i;
	char	*tmp;
	char	*cmd_path;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		if (!tmp)
			return (NULL);
		cmd_path = ft_strjoin(tmp, cmd_name);
		if (!cmd_path)
			return (free(tmp), NULL);
		free(tmp);
		if (!access(cmd_path, X_OK))
			return (errno = 0, cmd_path);
		free(cmd_path);
		i++;
	}
	return (errno = ENOENT, NULL);
}

char	*cmd_find_path(char *cmd_name, t_var *env_list)
{
	char	*cmd_path;
	char	**paths;

	cmd_path = check_abs_rel(cmd_name);
	if (cmd_path)
		return (cmd_path);
	if (errno)
		return (NULL);
	paths = unpack_path(env_search_name(env_list, "PATH"));
	if (!paths)
		return (NULL);
	cmd_path = check_paths(cmd_name, paths);
	if (cmd_path && !errno)
		return (cmd_path);
	return (NULL);
}
