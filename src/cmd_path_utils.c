/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efret <efret@student.19.be>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 16:02:06 by efret             #+#    #+#             */
/*   Updated: 2024/07/24 17:01:52 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	free_split(char **split)
{
	size_t	i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
		free(split[i++]);
	free(split);
}

static char	**unpack_path(t_var *path_var, t_minishell *shell)
{
	char	**paths;

	if (!path_var)
		return (errno = ENOENT, g_shell_stats.prev_exit = 127, NULL);
	paths = ft_split(path_var->value, ':');
	if (!paths)
		exit_handler(shell, errno);
	if (!(*paths))
		return (free(paths),
			errno = ENOENT, g_shell_stats.prev_exit = 127, NULL);
	return (paths);
}

static char	*check_abs_rel(char *cmd_name)
{
	char	*cmd_path;

	if (!access(cmd_name, F_OK))
	{
		if (!access(cmd_name, X_OK))
		{
			cmd_path = ft_strdup(cmd_name);
			if (!cmd_path)
				return (NULL);
			return (cmd_path);
		}
		perror(cmd_name);
		g_shell_stats.prev_exit = 126;
	}
	else
	{
		perror(cmd_name);
		g_shell_stats.prev_exit = 127;
	}
	return (NULL);
}

static char	*check_paths(char *cmd_name, char **paths)
{
	size_t	i;
	char	*tmp;
	char	*cmd_path;

	i = 0;
	if (!paths || !(paths[0]))
		return (NULL);
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i++], "/");
		if (!tmp)
			return (free_split(paths), NULL);
		cmd_path = ft_strjoin(tmp, cmd_name);
		if (!cmd_path)
			return (free(tmp), free_split(paths), NULL);
		if (!access(cmd_path, F_OK))
		{
			if (!access(cmd_path, X_OK))
				return (free(tmp), free_split(paths), errno = 0, cmd_path);
			return (perror(cmd_path), free(tmp), free_split(paths),
				g_shell_stats.prev_exit = 126, NULL);
		}
		(free(tmp), free(cmd_path));
	}
	return (free_split(paths), errno = 2, g_shell_stats.prev_exit = 127, NULL);
}

char	*cmd_find_path(char *cmd_name, t_var *env_list, t_minishell *shell)
{
	char	*cmd_path;
	char	**paths;

	if (!cmd_name || !cmd_name[0])
		return (errno = EINVAL, NULL);
	if (cmd_name[0] == '.' || ft_strchr(cmd_name, '/'))
		return (check_abs_rel(cmd_name));
	paths = unpack_path(env_search_name(env_list, "PATH"), shell);
	if (!paths)
		return (perror(cmd_name), NULL);
	cmd_path = check_paths(cmd_name, paths);
	if (cmd_path)
		return (cmd_path);
	ft_putstr_fd(cmd_name, STDERR_FILENO);
	ft_putendl_fd(": command not found", STDERR_FILENO);
	return (NULL);
}
