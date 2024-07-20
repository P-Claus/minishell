/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efret <efret@student.19.be>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 15:08:58 by efret             #+#    #+#             */
/*   Updated: 2024/07/20 17:00:28 by efret            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_between_incl(int val, int min, int max)
{
	if (min <= val && val <= max)
		return (1);
	return (0);
}

static int	ft_atoi_exit(char *s)
{
	int		sign;
	long	new_number;
	long	number;

	sign = 1;
	number = 0;
	if (*s == '+' || *s == '-')
		if (*s++ == '-')
			sign *= -1;
	while (*s)
	{
		if (!ft_between_incl(*s, '0', '9'))
			return (errno = EINVAL, 0);
		new_number = number * 10 + sign * (*s++ - '0');
		if ((sign == 1 && new_number < number)
			|| (sign == -1 && new_number > number))
			return (errno = EINVAL, 0);
		number = new_number;
	}
	return (number);
}

void	check_for_exit(t_cmd *cmds, t_minishell *shell)
{
	if (cmds && exact_match(cmds->cmd_av[0], "exit") && !cmds->next)
	{
		write(STDERR_FILENO, "exit\n", 5);
		if (!cmds->cmd_av[1])
			exit_handler(shell, -1);
		if (cmds->cmd_av[2])
		{
			g_shell_stats.prev_exit = 1;
			ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
			return ;
		}
		g_shell_stats.prev_exit = ft_atoi_exit(cmds->cmd_av[1]);
		if (errno == EINVAL)
		{
			g_shell_stats.prev_exit = 2;
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(cmds->cmd_av[1], STDERR_FILENO);
			ft_putendl_fd(": numeric argument required", STDERR_FILENO);
			exit_handler(shell, -1);
		}
		exit_handler(shell, -1);
	}
}
