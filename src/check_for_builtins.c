/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_for_builtins.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pclaus <pclaus@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 19:40:59 by pclaus            #+#    #+#             */
/*   Updated: 2024/07/11 15:06:07 by efret            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	builtin_wrapper(int (*func)(t_cmd *, t_minishell *), t_cmd *cmd,
				t_minishell *shell, int pipe_fd[2]);

static int	new_pwd(t_cmd *cmd, t_minishell *shell)
{
	char	*pwd_val;

	(void)cmd;
	(void)shell;
	pwd_val = getcwd(NULL, 0);
	printf("%s\n", pwd_val);
	free(pwd_val);
	return (0);
}

static int	new_echo(t_cmd *cmd, t_minishell *shell)
{
	size_t	iter;
	bool	nl;

	(void)shell;
	iter = 1;
	nl = true;
	if (cmd->cmd_av[iter] && exact_match(cmd->cmd_av[iter], "-n"))
	{
		nl = false;
		iter++;
	}
	while (cmd->cmd_av[iter])
	{
		printf("%s", cmd->cmd_av[iter++]);
		if (cmd->cmd_av[iter])
			printf(" ");
	}
	if (nl)
		printf("\n");
	return (0);
}

static int	new_env(t_cmd *cmd, t_minishell *shell)
{
	size_t	i;

	(void)cmd;
	i = 0;
	while (shell->export_env[i])
		printf("%s\n", shell->export_env[i++]);
	return (0);
}

static int	new_unset(t_cmd *cmd, t_minishell *shell)
{
	size_t	iter;
	t_var	*to_unset;

	iter = 1;
	while (cmd->cmd_av[iter])
	{
		to_unset = env_search_name(shell->env, cmd->cmd_av[iter]);
		env_del_target(&shell->env, to_unset);
		iter++;
	}
	env_update_export(shell);
	return (0);
}

/* Technically should print them sorted */
static int	export_print(t_minishell *shell)
{
	t_var	*var;

	var = shell->env;
	while (var)
	{
		if (var->is_exp)
		{
			printf("declare -x %s", var->name);
			if (var->value)
				printf("=\"%s\"", var->value);
			printf("\n");
		}
		var = var->next;
	}
	return (0);
}

static int	new_export(t_cmd *cmd, t_minishell *shell)
{
	size_t	iter;
	t_var	*var;

	iter = 1;
	if (!cmd->cmd_av[iter])
		return (export_print(shell));
	while (cmd->cmd_av[iter])
	{
		if (ft_strchr(cmd->cmd_av[iter], '='))
			var = env_add_var(&shell->env, cmd->cmd_av[iter], true);
		else
			var = env_add_var_only(&shell->env, cmd->cmd_av[iter], true);
		if (!var)
			break ;
		iter++;
	}
	env_update_export(shell);
	return (0);
}

static void	builtin_cd_no_arg(t_minishell *shell)
{
	t_var	*home_node;
	t_var	*pwd_node;

	home_node = env_search_name(shell->env, "HOME");
	if (!home_node)
	{
		printf("cd: HOME not set\n");
		old_exit_handler(1); //error handling
	}
	if (chdir(home_node->value))
	{
		if (errno == 2)
			printf("No such file or directory\n");
		g_shell_stats.prev_exit = 1;
		old_exit_handler(1); //error handling
	}
	pwd_node = env_search_name(shell->env, "PWD");
	if (pwd_node)
		env_add_var2(&shell->env, "OLDPWD", pwd_node->value, true);
	env_add_var2(&shell->env, "PWD", home_node->value, true);
	env_update_export(shell);
}

static void	builtin_cd_arg(t_cmd *cmd, t_minishell *shell)
{
	char	*pwd_value;
	t_var	*pwd_node;

	if (chdir(cmd->cmd_av[1]))
	{
		if (errno == 2)
			printf("No such file or directory\n");
		g_shell_stats.prev_exit = 1;
		old_exit_handler(1); //error handling
	}
	pwd_value = getcwd(NULL, 0);
	if (!pwd_value)
		old_exit_handler(1);//error handling
	pwd_node = env_search_name(shell->env, "PWD");
	if (pwd_node)
		env_add_var2(&shell->env, "OLDPWD", pwd_node->value, true);
	env_add_var2(&shell->env, "PWD", pwd_value, true);
	free(pwd_value);
	env_update_export(shell);
}
static int	new_cd(t_cmd *cmd, t_minishell *shell)
{
	if (!cmd->cmd_av[1])
		builtin_cd_no_arg(shell);
	else 
		builtin_cd_arg(cmd, shell);
	g_shell_stats.prev_exit = 0;
	return (0);
}

int	check_for_builtins(t_cmd *cmd, t_minishell *shell, int pipe_fd[2])
{
	(void)pipe_fd;
	if (exact_match(cmd->cmd_av[0], "echo"))
		builtin_wrapper(new_echo, cmd, shell, pipe_fd);
	else if (exact_match(cmd->cmd_av[0], "cd"))
		builtin_wrapper(new_cd, cmd, shell, pipe_fd);
	else if (exact_match(cmd->cmd_av[0], "pwd"))
		builtin_wrapper(new_pwd, cmd, shell, pipe_fd);
	else if (exact_match(cmd->cmd_av[0], "export"))
		builtin_wrapper(new_export, cmd, shell, pipe_fd);
	else if (exact_match(cmd->cmd_av[0], "unset"))
		builtin_wrapper(new_unset, cmd, shell, pipe_fd);
	else if (exact_match(cmd->cmd_av[0], "env"))
		builtin_wrapper(new_env, cmd, shell, pipe_fd);
	else if (exact_match(cmd->cmd_av[0], "exit"))
		printf("Exit Builtin\n");
	else
		return (0);
	return (1);
}
static void	builtin_wrapper(int (*func)(t_cmd *, t_minishell *), t_cmd *cmd,
		t_minishell *shell, int pipe_fd[2])
{
	int	stdout_copy;

	stdout_copy = dup(STDOUT_FILENO);
	if (cmd->next && dup2(pipe_fd[PIPE_W], STDOUT_FILENO) == -1)
		old_exit_handler(1); //error
	do_redirs(cmd);
	g_shell_stats.prev_exit = func(cmd, shell);
	close(pipe_fd[PIPE_W]);
	if (dup2(pipe_fd[PIPE_R], STDIN_FILENO) == -1 || close(pipe_fd[PIPE_R]))
		old_exit_handler(1);
	if (dup2(stdout_copy, STDOUT_FILENO) == -1 || close(stdout_copy))
		old_exit_handler(1);
}
