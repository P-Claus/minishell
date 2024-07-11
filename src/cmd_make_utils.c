/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_make_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efret <efret@student.19.be>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 16:48:52 by efret             #+#    #+#             */
/*   Updated: 2024/07/11 15:05:12 by efret            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

size_t	count_cmd_av(t_token *tokens)
{
	size_t	count;

	count = 0;
	while (tokens && tokens->tag != PIPE)
	{
		if (tokens->tag != REDIRECT)
			count++;
		else if (tokens->next)
			tokens = tokens->next;
		else
			(printf("Welp\n"), old_exit_handler(1)); // Unfinished Redirect, maybe better to catch while pre_parsing?
		tokens = tokens->next;
	}
	return (count);
}

void	add_new_redir_node(t_redir **redirs, t_token **tokens)
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
		old_exit_handler(errno);//handle malloc error
	redir_add_back(redirs, redir);
}

void	fill_cmd_content(char **cmd_av, t_redir **redirs, t_token **tokens)
{
	size_t	i;

	i = 0;
	while (*tokens && (*tokens)->tag != PIPE)
	{
		if ((*tokens)->tag != REDIRECT)
			cmd_av[i++] = (*tokens)->str;
		else
			add_new_redir_node(redirs, tokens);
		if (!(*tokens))
			(printf("Unfinished Redirect\n"), exit(1)); // Error maybe better to be caught in pre_parser
		*tokens = (*tokens)->next;
	}
}

void	make_cmd_list(t_cmd **cmds, t_token *tokens)
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
			(printf("Only redir info\n"), old_exit_handler(1)); //error, only redirect info
		cmd_av = malloc(sizeof(char *) * (count_av + 1));
		if (!cmd_av)
			old_exit_handler(1); //malloc failure
		cmd_av[count_av] = NULL;
		fill_cmd_content(cmd_av, &redirs, &tokens);
		cmd_node = create_cmd(cmd_av, redirs);
		if (!cmd_node)
			old_exit_handler(errno); //again malloc failure
		cmd_add_back(cmds, cmd_node);
		if (tokens)
			tokens = tokens->next;
	}
}
