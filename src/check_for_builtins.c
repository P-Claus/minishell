/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_for_builtins.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pclaus <pclaus@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 19:40:59 by pclaus            #+#    #+#             */
/*   Updated: 2024/06/06 17:53:45 by pclaus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_for_builtins(char *string, char **envp)
{
	if (exact_match(string, "echo"))
		printf("You have entered the echo builtin\n");
	else if (exact_match(string, "cd"))
		builtin_cd(envp);
	else if (exact_match(string, "pwd"))
		builtin_pwd(envp);
	else if (exact_match(string, "export"))
		printf("You have entered the EXPORT builtin\n");
	else if (exact_match(string, "unset"))
		printf("You have entered the UNSET builtin\n");
	else if (exact_match(string, "env"))
		builtin_env(envp);
	else if (exact_match(string, "exit"))
		printf("You have entered the EXIT builtin\n");
	return (0);
}

void	builtin_pwd(char **envp)
{
	char	*current_working_directory;
	int	iter;

	iter = 0;
	while (envp[iter])
	{
		if (ft_strnstr(envp[iter], "PWD=", 200))
		{
			current_working_directory = ft_strchr(envp[iter], '=');
			current_working_directory = current_working_directory + 1;
			printf("%s\n", current_working_directory);
			break;
		}
		iter++;
	}
//	exit_handler(19); //temporary excit code
}

void	builtin_env(char **envp)
{
	int	iter;

	iter = 0;
	while (envp[iter])
	{
		if (envp[iter][0])
		{
			printf("%s\n", envp[iter]);
			iter++;
		}
	}
	exit_handler(20); //temporary exit code
}

void	builtin_cd(char **envp)
{
	char	*home_path;
	int		iter;
	int		iter_path;
	char	*current_working_directory;
	char	*new_pwd_path;

	iter = 0;
	iter_path = 0;
	printf("-------------------------------------\n");
	while (envp[iter_path])
	{
		if (ft_strnstr(envp[iter_path], "PWD=", 200))
		{
			printf("The pwd is: %s\n", envp[iter_path]);
			break ;
		}
		iter_path++;
	}
	while (envp[iter])
	{
		if (ft_strnstr(envp[iter], "HOME=", 100))
		{
			printf("The home path is: %s\n", envp[iter]);
			home_path = ft_strchr(envp[iter], '=');
			home_path = home_path + 1;
			printf("The stripped home path is: %s\n", home_path);
			chdir(home_path);
			current_working_directory = getenv("PWD");
			printf("%s\n", current_working_directory);
			break ;
		}
		else
		{
			printf("minihell: cd: HOME not set\n");
			exit_handler(23);//temporary exit handler
		}
		iter++;
	}
	new_pwd_path = ft_strjoin("PWD=", home_path);
	printf("The test is: %s\n", home_path);
	printf("The newly joined string is: %s\n", new_pwd_path);
	envp[iter_path] = new_pwd_path;
	printf("-------------------------------------\n");
	exit_handler(21); //temporary exit code
}
