/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efret <efret@student.19.be>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 17:51:54 by efret             #+#    #+#             */
/*   Updated: 2024/07/24 17:07:08 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	put_here_doc_warning(char *delim)
{
	ft_putstr_fd("minishell: warning: here-document ", STDERR_FILENO);
	ft_putstr_fd("delimited by end-of-file (wanted `", STDERR_FILENO);
	ft_putstr_fd(delim, STDERR_FILENO);
	ft_putendl_fd("')", STDERR_FILENO);
}

static void	read_here_doc(
		t_minishell *shell, int pipe_fd[2], t_redir *redir, bool expand)
{
	char	*line;

	(void)shell;
	handle_sigint_heredoc();
	close(pipe_fd[PIPE_R]);
	line = readline("> ");
	while (line)
	{
		if (exact_match(line, redir->str))
			break ;
		if (expand)
			process_token(&line, shell);
		ft_putendl_fd(line, pipe_fd[PIPE_W]);
		free(line);
		line = readline("> ");
	}
	close(pipe_fd[PIPE_W]);
	if (line)
		free(line);
	else if (!errno)
		put_here_doc_warning(redir->str);
	exit_handler(shell, errno);
}

static void	here_doc_fork(
		t_minishell *shell, int pipe_fd[2], t_redir *redir, bool expand)
{
	int	cpid;

	cpid = fork();
	if (cpid == -1)
		exit_handler(shell, errno);
	if (!cpid)
		read_here_doc(shell, pipe_fd, redir, expand);
	close(pipe_fd[PIPE_W]);
	ft_wait(cpid);
}

void	parse_here_docs(t_minishell *shell, t_cmd *cmds, int pipe_fd[2])
{
	t_redir	*redirs;

	while (cmds)
	{
		redirs = cmds->redirs;
		while (redirs)
		{
			if (redirs->flags == R_HERE || redirs->flags == R_HERE_NO_EXP)
			{
				g_shell_stats.process_is_running = 1;
				if (pipe(pipe_fd) == -1)
					exit_handler(shell, errno);
				here_doc_fork(shell, pipe_fd, redirs, redirs->flags == R_HERE);
				if (g_shell_stats.prev_exit)
					return (close(pipe_fd[PIPE_R]), (void)0);
				redirs->flags = R_HERE;
				redirs->fd = pipe_fd[PIPE_R];
				redirs->is_fd = true;
			}
			redirs = redirs->next;
		}
		cmds = cmds->next;
	}
}
