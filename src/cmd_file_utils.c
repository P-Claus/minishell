/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_file_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efret <efret@student.19.be>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 14:19:19 by efret             #+#    #+#             */
/*   Updated: 2024/07/23 14:39:12 by efret            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	open_file(char *name, int flag)
{
	int	fd;

	if (flag == R_IN)
		fd = open(name, O_RDONLY | O_CLOEXEC);
	else if (flag == R_OUT)
		fd = open(name, O_WRONLY | O_TRUNC | O_CLOEXEC | O_CREAT, 00666);
	else if (flag == R_APND)
		fd = open(name, O_WRONLY | O_APPEND | O_CLOEXEC | O_CREAT, 00666);
	else
		return (errno = EINVAL, -1);
	return (fd);
}

void	do_redirs(t_cmd *cmd, t_minishell *shell)
{
	t_redir	*redirs;
	int		fd;

	redirs = cmd->redirs;
	while (redirs)
	{
		if (redirs->flags == R_HERE && redirs->is_fd)
		{
			if (dup2(redirs->fd, STDIN_FILENO) == -1)
				exit_handler(shell, errno);
			close(redirs->fd);
		}
		else
		{
			fd = open_file(redirs->str, redirs->flags);
			if (errno)
				exit_handler(shell, errno);
			redirs->fd = fd;
			redirs->is_fd = true;
			if (dup2(fd, (redirs->flags != R_IN) * 1) == -1)
				exit_handler(shell, errno);
		}
		redirs = redirs->next;
	}
}

void	close_redirs(t_cmd *cmds)
{
	t_cmd	*cmd;
	t_redir	*redirs;

	cmd = cmds;
	while (cmd)
	{
		redirs = cmd->redirs;
		while (redirs)
		{
			if (redirs->fd > 0)
			{
				close(redirs->fd);
				redirs->is_fd = false;
			}
			redirs = redirs->next;
		}
		cmd = cmd->next;
	}
}
