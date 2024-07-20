/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pclaus <pclaus@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 18:32:43 by pclaus            #+#    #+#             */
/*   Updated: 2024/07/20 15:50:37 by efret            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_shell_stats	g_shell_stats;

void	non_interactive(char *filename, t_minishell *shell)
{
	int		file;
	char	*line;
	t_token	*tokens;
	t_cmd	*cmds;

	file = open(filename, O_RDONLY);
	if (file == -1)
		return ;
	line = get_next_line(file);
	while (line)
	{
		cmds = NULL;
		tokens = lexer(line, shell);
		make_cmd_list(&cmds, tokens);
		ft_run_cmds(cmds, shell);
		free_cmds(&cmds);
		free_tokens(&tokens);
		free(line);
		line = get_next_line(file);
	}
	exit(0);
}

void	load_rc(char *filename, t_minishell *shell)
{
	int		file;
	char	*line;
	t_token	*tokens;
	t_cmd	*cmds;

	file = open(filename, O_RDONLY);
	if (file == -1)
		return ;
	line = get_next_line(file);
	while (line)
	{
		cmds = NULL;
		tokens = lexer(line, shell);
		make_cmd_list(&cmds, tokens);
		ft_run_cmds(cmds, shell);
		free_cmds(&cmds);
		free_tokens(&tokens);
		free(line);
		line = get_next_line(file);
	}
	close(file);
}

int	main(int ac, char **av, char **envp)
{
	t_minishell	shell;

	if (ac > 2)
		exit(0);
	shell_init(&shell, envp);
	if (ac == 2)
		non_interactive(av[1], &shell);
	load_rc(".minishellrc", &shell);
	interactive(&shell);
	return (0);
}
