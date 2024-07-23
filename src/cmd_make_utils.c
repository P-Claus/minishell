/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_make_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efret <efret@student.19.be>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 16:48:52 by efret             #+#    #+#             */
/*   Updated: 2024/07/20 19:11:06 by efret            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static size_t	count_cmd_av(t_token *tokens)
{
	size_t	count;

	count = 0;
	while (tokens && tokens->tag != PIPE)
	{
		if (tokens->tag != REDIRECT)
			count++;
		else if (tokens->next)
		{
			tokens = tokens->next;
			if (tokens->tag == REDIRECT)
				return (ft_putendl_fd("minishell: syntax error", STDERR_FILENO),
					g_shell_stats.prev_exit = 2, 0);
		}
		else
			return (ft_putendl_fd("minishell: syntax error", STDERR_FILENO),
				g_shell_stats.prev_exit = 2, 0);
		tokens = tokens->next;
	}
	return (count);
}

static void	add_new_redir_node(t_minishell *shell,
		t_redir **redirs, t_token **tokens)
{
	t_redir	*redir;
	char	*redir_op;
	char	*redir_name;
	int		flags;

	redir_op = (*tokens)->str;
	*tokens = (*tokens)->next;
	redir_name = (*tokens)->str;
	flags = R_NONE;
	if (exact_match(redir_op, "<"))
		flags = R_IN;
	else if (exact_match(redir_op, ">"))
		flags = R_OUT;
	else if (exact_match(redir_op, "<<")
		&& ((*tokens)->tag == SINGLE_Q || (*tokens)->tag == DOUBLE_Q))
		flags = R_HERE_NO_EXP;
	else if (exact_match(redir_op, "<<"))
		flags = R_HERE;
	else if (exact_match(redir_op, ">>"))
		flags = R_APND;
	redir = create_redir(redir_name, flags);
	if (!redir)
		exit_handler(shell, errno);
	redir_add_back(redirs, redir);
}

static int	fill_cmd_content(t_minishell *shell,
		char **cmd_av, t_redir **redirs, t_token **tokens)
{
	size_t	i;

	i = 0;
	while (*tokens && (*tokens)->tag != PIPE)
	{
		if ((*tokens)->tag != REDIRECT)
			cmd_av[i++] = (*tokens)->str;
		else
			add_new_redir_node(shell, redirs, tokens);
		if (!(*tokens))
			return (ft_putendl_fd("Unfinished Redirect", STDERR_FILENO), 1);
		*tokens = (*tokens)->next;
	}
	return (0);
}

int	make_cmd_list(t_minishell *shell, t_cmd **cmds, t_token *tokens)
{
	t_cmd	*cmd_node;
	char	**cmd_av;
	size_t	count_av;
	t_redir	*redirs;

	while (tokens)
	{
		redirs = NULL;
		count_av = count_cmd_av(tokens);
		if (!count_av)
			return (1);
		cmd_av = malloc(sizeof(char *) * (count_av + 1));
		if (!cmd_av)
			exit_handler(shell, errno);
		cmd_av[count_av] = NULL;
		if (fill_cmd_content(shell, cmd_av, &redirs, &tokens))
			return (free(cmd_av), 1);
		cmd_node = create_cmd(cmd_av, redirs);
		if (!cmd_node)
			(free(cmd_av), exit_handler(shell, errno));
		cmd_add_back(cmds, cmd_node);
		if (tokens)
			tokens = tokens->next;
	}
	return (0);
}
