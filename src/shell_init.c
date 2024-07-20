/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efret <efret@student.19.be>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 15:50:24 by efret             #+#    #+#             */
/*   Updated: 2024/07/20 16:56:57 by efret            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	init_shell_lvl(t_minishell *shell)
{
	int		lvl;
	char	*tmp;
	t_var	*var;

	var = env_search_name(shell->env, "SHLVL");
	if (!var && !env_add_var2(&shell->env, "SHLVL", "1", true))
		exit_handler(shell, -1);
	else if (!var)
		return ;
	lvl = ft_atoi(var->value) + 1;
	if (lvl > 999)
	{
		printf(
			"minishell: Warning: shell level (%i) too high, resetting to 1\n",
			lvl);
		lvl = 1;
	}
	if (lvl < 0)
		lvl = 0;
	tmp = ft_itoa(lvl);
	if (!tmp)
		return ;
	if (!env_add_var2(&shell->env, "SHLVL", tmp, true))
		(free(tmp), exit_handler(shell, -1));
	return (free(tmp));
}

static void	init_old_pwd(t_minishell *shell)
{
	if (!env_search_name(shell->env, "OLDPWD"))
		if (!env_add_var2(&shell->env, "OLDPWD", NULL, true))
			exit_handler(shell, -1);
}

static void	init_pwd(t_minishell *shell)
{
	char	*pwd_val;
	t_var	*var;

	pwd_val = getcwd(NULL, 0);
	if (!pwd_val)
		exit_handler(shell, -1);
	var = env_add_var2(&shell->env, "PWD", pwd_val, true);
	free(pwd_val);
	if (!var)
		exit_handler(shell, -1);
}

static void	init_path(t_minishell *shell)
{
	char	*dft_p;

	dft_p = "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin";
	if (!env_search_name(shell->env, "PATH"))
		if (!env_add_var2(&shell->env, "PATH", dft_p, false))
			exit_handler(shell, -1);
}

void	shell_init(t_minishell *shell, char **envp)
{
	ft_memset(shell, 0, sizeof(t_minishell));
	env_load(shell, envp);
	init_shell_lvl(shell);
	init_pwd(shell);
	init_old_pwd(shell);
	init_path(shell);
	ft_memset(&g_shell_stats, 0, sizeof(t_shell_stats));
	shell->export_env = make_export_envp(shell);
}
