/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 14:39:40 by saylital          #+#    #+#             */
/*   Updated: 2025/01/16 19:09:36 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_cd(char **command, t_ms *shell)
{
	char	*oldpwd;
	char	*home;
	int		count;

	oldpwd = getcwd(NULL, 0);
	home =  getenv("HOME");
	count = count_args(command);
	if (!oldpwd)
	{
		free_args(command);
		free_env(shell);
		print_error("getcwd failed", shell, 1);
		exit(shell->exit_code);
	}
	if (count > 2)
	{
		print_error("minishell: cd: too many arguments", shell, 1);
		return ;
	}
	update_pwd(shell, "OLDPWD=", oldpwd);
	if (count == 1)
	{
		if (chdir(home) == -1)
			//need to free stuff and setup correct exit code if home not found
			perror("");
		return ;
	}
	else
	{
		if (chdir(command[1]) == -1)
		{
			perror("");
			//need to free stuff
			shell->exit_code = 1;
		}
		return ;
	}
}
