/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pclaus <pclaus@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 16:09:16 by pclaus            #+#    #+#             */
/*   Updated: 2024/06/18 16:13:43 by efret            ###   ########.fr       */
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
		rl_set_prompt("\033[31m->\033[0m minishell> ");// Only needed for the red arrow
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
