/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pclaus <pclaus@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 18:32:55 by pclaus            #+#    #+#             */
/*   Updated: 2024/07/24 15:35:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/incl/libft.h"
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>

# define PIPE_R 0
# define PIPE_W 1

# define SH_INTR 1
# define SH_CMD	2

typedef enum e_token_type
{
	CMD,
	PIPE,
	REDIRECT,
	MAKE_VAR,
	MAKE_VAR_SINGLE,
	MAKE_VAR_DOUBLE,
	SINGLE_Q,
	DOUBLE_Q,
}	t_token_type;

typedef enum e_redir_type
{
	R_NONE,
	R_IN,
	R_OUT,
	R_APND,
	R_HERE,
	R_HERE_NO_EXP,
}	t_redir_type;

typedef enum e_lexing_state
{
	START,
	UNQUOTED,
	SQ,
	DQ,
	SP,
	META,
	DOLLAR,
	VAR_SINGLE,
	VAR_DOUBLE,
}	t_lexing_state;

/*   STRUCTURES   */
typedef struct s_token
{
	char			*str;
	t_token_type	tag;
	struct s_token	*next;
}	t_token;

typedef struct s_lexeme
{
	char			*string;
	t_token			*head;
	t_lexing_state	lexing_state;
}	t_lexeme;

typedef struct s_redir
{
	char			*str;
	int				fd;
	int				flags;
	bool			is_fd;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**cmd_av;
	t_redir			*redirs;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_var
{
	char			*name;
	char			*value;
	bool			is_exp;
	struct s_var	*next;
	struct s_var	*prev;
}	t_var;

typedef struct s_minishell
{
	char	*line;
	t_cmd	*cmds;
	t_token	*tokens;
	t_var	*env;
	char	**export_env;
}	t_minishell;

typedef struct s_shell_stats
{
	volatile sig_atomic_t	prev_exit;
	volatile sig_atomic_t	process_is_running;
}	t_shell_stats;

extern t_shell_stats	g_shell_stats;

typedef struct s_expand_string_info
{
	char	*trimmed_parameter;
	char	*expanded_string;
	int		start;
	int		end;
	char	*env_value;
	bool	free_env_value;
}	t_string_info;

/* MAIN LOOP */
void	shell_init(t_minishell *shell, char **envp);
void	interactive(t_minishell *shell);

/*	BUILTINS	*/
int		new_cd(t_cmd *cmd, t_minishell *shell);
int		new_pwd(t_cmd *cmd, t_minishell *shell);
int		new_echo(t_cmd *cmd, t_minishell *shell);
int		new_env(t_cmd *cmd, t_minishell *shell);
int		new_unset(t_cmd *cmd, t_minishell *shell);
int		new_export(t_cmd *cmd, t_minishell *shell);
void	check_for_exit(t_cmd *cmds, t_minishell *shell);
void	builtin_exit(void);

/*	UTILS	*/
bool	exact_match(char *s, char *to_match);
void	old_exit_handler(int status);
void	exit_handler(t_minishell *shell, int status);
int		is_regular_character(char c);
int		is_meta_character(char c);
void	reset_lexer_state(t_lexeme *lexeme, t_lexing_state lexing_state);
int		ft_strjoin_char(char **str, char c);
void	calculate_start_and_end(char **string, int *start, int *end, int iter);
char	*get_trimmed_parameter(
			int start, int end, char **string, t_minishell *shell);
char	*get_expanded_string(
			t_string_info *s_info, char **string, t_minishell *shell);
char	*get_env_value(
			t_var *env, char *name, bool has_braces, t_minishell *shell);
void	expand_string(
			char **string, t_minishell *shell, t_string_info *s_info, int iter);

/*	LINKED LIST FUNCTIONS	*/
t_token	*create_token(char *string);
void	add_token_to_end(t_token **head, t_token *new_token);
void	print_list(t_token **token);
t_cmd	*create_cmd(char **cmd_av, t_redir *redirs);
void	cmd_add_back(t_cmd **head, t_cmd *new_node);
t_redir	*create_redir(char *str, int flags);
void	redir_add_back(t_redir **head, t_redir *new_node);

/* COMMANDS */
int		make_cmd_list(t_minishell *shell, t_cmd **cmds, t_token *tokens);
char	*cmd_find_path(char *cmd_name, t_var *env_list, t_minishell *shell);
void	do_redirs(t_cmd *cmd, t_minishell *shell);
void	close_redirs(t_cmd *cmd);
void	check_for_leading_vars(t_cmd *cmd, t_minishell *shell);
int		check_for_only_vars(t_cmd *cmds, t_minishell *shell);
void	ft_run_cmds(t_cmd *cmds, t_minishell *shell);
void	ft_wait(pid_t cpid);

/* HERE_DOC */
void	parse_here_docs(t_minishell *shell, t_cmd *cmds, int pipe_fd[2]);

/* ENVIRONMENT VARIABLE */
t_var	*create_env_var(char *name, char *val, bool is_exp);
int		env_add_back(t_var **head, t_var *new_node);
void	env_del_target(t_var **head, t_var *node);
t_var	*env_search_name(t_var *head, char *name);
void	env_load(t_minishell *shell, char **envp);
t_var	*env_add_var2(t_var **head, char *name, char *value, bool exp);
t_var	*env_add_var(t_var **head, char *token, bool is_exp);
t_var	*env_add_var_only(t_var **env, char *name, bool is_exp);
char	**make_export_envp(t_minishell *shell);
void	env_update_export(t_minishell *shell);

/* FREE UTILS */
void	free_minishell(t_minishell *shell);
void	free_tokens(t_token **tokens);
void	free_cmds(t_cmd **cmds);
void	free_env(t_var **env);
void	free_env_export(char ***exp);

/*	SRC	*/
int		check_for_builtins(t_cmd *cmd, t_minishell *shell, int pipe_fd[2]);
t_token	*lexer(char *line, t_minishell *shell);
void	process_token(char **string, t_minishell *shell);
void	expand_double_quotes(char **string, t_minishell *shell);
void	handle_single_quotes(t_lexeme *lexeme, char *line, int *index);
void	handle_unquoted(t_lexeme *lexeme, char *line, int *index);
void	update_lexer_state(t_lexeme *lexeme, char *line, int *index);
void	handle_meta_char(t_lexeme *lexeme, char *line, int *index);
void	handle_space(t_lexeme *lexeme, char *line, int *index);
void	handle_double_quotes(t_lexeme *lexeme, char *line, int *index);
void	handle_dollar(t_lexeme *lexeme, char *line, int *index);
void	handle_var_single(t_lexeme *lexeme, char *line, int **index);
void	handle_var_double(t_lexeme *lexeme, char *line, int **index);
void	handle_var_make(t_lexeme *lexeme, char *line, int **index);
void	parser(t_token **token);
void	remove_quotes_from_quoted_string(t_token **token);
void	remove_quotes_from_variables(t_token **token, t_minishell *shell);
void	expand_parameters(t_token **token, t_minishell *shell);
void	handle_sigint(void);
void	handle_sigquit(void);
void	handle_sigquit_child(void);
void	handle_sigint_heredoc(void);
void	handle_eof(char *line, int index);
void	handle_sigchld(void);
bool	valid_var_name(char *s);
bool	valid_var_token(char *s);
#endif
