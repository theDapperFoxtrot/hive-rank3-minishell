/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 14:39:40 by saylital          #+#    #+#             */
/*   Updated: 2025/03/10 15:10:05 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	cd_to_arg(char **command, t_ms *shell)
{
	if (chdir(command[1]) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(command[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		shell->exit_code = 1;
		return (1);
	}
	return (0);
}

void	error_and_exit_code(t_ms *shell)
{
	ft_putstr_fd("minishell: cd: HOME not set", 2);
	shell->exit_code = 1;
}

void	ft_cd(char **command, t_ms *shell)
{
	char	*oldpwd;
	char	*home;
	int		count;
	char	*temp;


	oldpwd = getcwd(NULL, 0);
	// CHECK GETENV WHEN WE IMPLEMENT UNSET; MAKING SURE IT FETCHES THE RIGHT ENV
	home =  getenv("HOME");
	count = count_args(command);
	if (!oldpwd)
	{
		if (ft_strncmp(command[1], "..", 2) == 0)
		{
			ft_putstr_fd("cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n", 2);
			shell->exit_code = 0;
			return ;
		}
		if (chdir(home) == -1)
			error_and_exit_code(shell);
		oldpwd = getcwd(NULL, 0);
		update_pwd(shell, "OLDPWD=", shell->prev_pwd);
		update_pwd(shell, "PWD=", oldpwd);
		return ;
	}
	if (shell->prev_pwd)
		free(shell->prev_pwd);
	shell->prev_pwd = ft_strdup(oldpwd);
	if (count > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		free(oldpwd);
		shell->exit_code = 1;
		return ;
	}
	if (shell->pipe_count > 0)
	{
		free(oldpwd);
		shell->exit_code = 1;
		// if (shell->prev_pwd)
		// 	free(shell->prev_pwd);
		return ;
	}
	if (count == 1)
	{
		if (chdir(home) == -1)
			error_and_exit_code(shell);
		free(oldpwd);
	}
	else
	{
		update_pwd(shell, "OLDPWD=", oldpwd);
		if (cd_to_arg(command, shell) == 0 && ft_strncmp(command[1], "..", 2) != 0)
		{
			temp = ft_strjoin(shell->prev_pwd ,"/");
			free(shell->prev_pwd);  // Free the old allocation
			shell->prev_pwd = ft_strjoin(temp , command[1]);
			free(temp);
		}
	}
	oldpwd = getcwd(NULL, 0);
	update_pwd(shell, "PWD=", oldpwd);
}
