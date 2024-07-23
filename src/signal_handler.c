/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pclaus <pclaus@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 16:09:16 by pclaus            #+#    #+#             */
/*   Updated: 2024/07/23 21:26:51 by pclaus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	custom_sigint_handler_function(int signal)
{
	(void)signal;
	if (!g_shell_stats.process_is_running)
	{
		write(1, "\n", 1);
		g_shell_stats.prev_exit = EOWNERDEAD;
		rl_replace_line("", 0);
		rl_set_prompt("\001\033[31m\002->\001\033[0m\002 minishell> ");
		rl_on_new_line();
		rl_redisplay();
	}
}

void	handle_sigint(void)
{
	struct sigaction	sa;
	sigset_t			signal_set;

	memset(&sa, 0, sizeof(sa));
	sigemptyset(&signal_set);
	sa.sa_mask = signal_set;
	sigaddset(&signal_set, SIGCHLD);
	sa.sa_handler = &custom_sigint_handler_function;
	sigaction(SIGINT, &sa, NULL);
}

void	handle_sigint_heredoc(void)
{
	struct sigaction	sa;
	sigset_t			signal_set;

	memset(&sa, 0, sizeof(sa));
	sigemptyset(&signal_set);
	sa.sa_mask = signal_set;
	sigaddset(&signal_set, SIGCHLD);
	sa.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa, NULL);
}

void	handle_sigquit(void)
{
	struct sigaction	sa;
	sigset_t			signal_set;

	ft_memset(&sa, 0, sizeof(sa));
	sigemptyset(&signal_set);
	sa.sa_mask = signal_set;
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

void	handle_sigquit_child(void)
{
	struct sigaction	sa;
	sigset_t			signal_set;

	ft_memset(&sa, 0, sizeof(sa));
	sigemptyset(&signal_set);
	sa.sa_mask = signal_set;
	sa.sa_handler = SIG_DFL;
	sigaction(SIGQUIT, &sa, NULL);
}
