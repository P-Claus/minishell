/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wait.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efret <efret@student.19.be>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 14:17:46 by efret             #+#    #+#             */
/*   Updated: 2024/07/23 14:18:33 by efret            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static inline void	status_stuff(pid_t cpid, pid_t pid, int wstat)
{
	if (pid != cpid)
		return ;
	if (WIFEXITED(wstat))
		g_shell_stats.prev_exit = WEXITSTATUS(wstat);
	else if (WIFSIGNALED(wstat))
	{
		if (WCOREDUMP(wstat))
		{
			printf("Quit (core dumped)\n");
			g_shell_stats.prev_exit = ENOTRECOVERABLE;
		}
		else if (WTERMSIG(wstat) == CLD_KILLED)
		{
			write(1, "\n", 1);
			g_shell_stats.prev_exit = EOWNERDEAD;
		}
	}
}

void	ft_wait(pid_t cpid)
{
	int		wstat;
	pid_t	pid;

	pid = wait(&wstat);
	while (pid > 0)
	{
		status_stuff(cpid, pid, wstat);
		pid = wait(&wstat);
	}
	if (errno == EINTR)
	{
		pid = waitpid(cpid, &wstat, 0);
		status_stuff(cpid, pid, wstat);
	}
	else if (errno == ECHILD)
		errno = 0;
	g_shell_stats.process_is_running = 0;
}
