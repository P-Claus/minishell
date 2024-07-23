/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactive.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efret <efret@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 14:45:03 by efret             #+#    #+#             */
/*   Updated: 2024/07/23 23:11:42 by efret            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static inline void	set_prompt(char **prompt)
{
	if (g_shell_stats.prev_exit)
		*prompt = "\001\033[31m\002->\001\033[0m\002 minishell> ";
	else
		*prompt = "\001\033[32m\002->\001\033[0m\002 minishell> ";
}

static inline void	process_line(t_minishell *shell)
{
	if (!shell->line)
	{
		write(STDERR_FILENO, "exit\n", 5);
		exit_handler(shell, -1);
	}
	if (shell->line && *shell->line)
	{
		add_history(shell->line);
		shell->tokens = lexer(shell->line, shell);
		if (make_cmd_list(shell, &shell->cmds, shell->tokens))
			return (free_tokens(&shell->tokens));
		ft_run_cmds(shell->cmds, shell);
		close_redirs(shell->cmds);
		free_cmds(&shell->cmds);
		free_tokens(&shell->tokens);
	}
}

void	interactive(t_minishell *shell)
{
	char	*prompt;

	handle_sigint();
	handle_sigquit();
	while (1)
	{
		set_prompt(&prompt);
		shell->cmds = NULL;
		shell->line = readline(prompt);
		process_line(shell);
		free(shell->line);
	}
	exit_handler(shell, -1);
}

/*
static void	disp_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("Token type %i: %s\n", tokens->tag, tokens->str);
		tokens = tokens->next;
	}
}

static void print_redir_node(t_redir_type flags, const char *str)
{
	if (flags == R_IN)
		printf("(%s, %s)", "IN", str);
	else if (flags == R_OUT)
		printf("(%s, %s)", "OUT", str);
	else if (flags == R_APND)
		printf("(%s, %s)", "APND", str);
	else if (flags == R_HERE)
		printf("(%s, %s)", "HERE", str);
}

static void	print_redir_list(t_redir *redirs)
{
	while (redirs)
	{
		print_redir_node(redirs->flags, redirs->str);
		if (redirs->next)
			printf(" -> ");
		redirs = redirs->next;
	}
}

static void	print_cmd_list(t_cmd *cmds)
{
	int	count;

	while (cmds)
	{
		count = printf("Command: ");
		for (int i = 0; cmds->cmd_av[i]; i++)
			count += printf("%s ", cmds->cmd_av[i]);
		printf("%*sRedirs: ", 30 - count, "");
		print_redir_list(cmds->redirs);
		printf("\n");
		cmds = cmds->next;
	}
}
*/

/*
void	interactive(t_minishell *shell)
{
	char	*prompt;

	handle_sigint();
	handle_sigquit();
	while (1)
	{
		if (g_shell_stats.prev_exit)
			prompt = "\001\033[31m\002->\001\033[0m\002 minishell> ";
		else
			prompt = "\001\033[32m\002->\001\033[0m\002 minishell> ";
		shell->cmds = NULL;
		shell->line = readline(prompt);
		if (!shell->line)
		{
			write(STDERR_FILENO, "exit\n", 5);
			exit_handler(shell, errno);//to handle when control D is entered (= EOT)
		}
		if (shell->line && *shell->line)
		{
			add_history(shell->line);

			printf("You have entered: %s\n", shell->line);
			shell->tokens = lexer(shell->line, shell);
			printf("\nIn Interactive\n");
			disp_tokens(shell->tokens);
			if (make_cmd_list(shell, &shell->cmds, shell->tokens))
			{
				free(shell->line);
				free_tokens(&shell->tokens);
				continue ;
			}
			printf("\nCommand list\n");
			print_cmd_list(shell->cmds);
			printf("\nCommand output:\n");
			ft_run_cmds(shell->cmds, shell);
			close_redirs(shell->cmds);
			free_cmds(&shell->cmds);
			free_tokens(&shell->tokens);
		}
		free(shell->line);
	}
	exit_handler(shell, -1);
}
*/
