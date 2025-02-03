/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 14:39:40 by saylital          #+#    #+#             */
/*   Updated: 2025/02/03 16:25:57 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void cd_to_arg(char **command, t_ms *shell)
{
	if (chdir(command[1]) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(command[1], 2);
		ft_putstr_fd(": no such file or directory\n", 2);
		shell->exit_code = 1;
	}
}

void	free_oldpwd_and_error(t_ms *shell, char *oldpwd, char* message)
{
	free(oldpwd);
	{
		print_error(message, shell, 1, 1);
		exit(shell->exit_code);
	}
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


	oldpwd = getcwd(NULL, 0);
	// CHECK GETENV WHEN WE IMPLEMENT UNSET; MAKING SURE IT FETCHES THE RIGHT ENV
	home =  getenv("HOME");
	count = count_args(command);
	if (!oldpwd)
		free_oldpwd_and_error(shell, oldpwd, "minishell: cd: getcwd failed");
	if (count > 2)
	{
		print_error("minishell: cd: too many arguments", shell, 1, 0);
		free(oldpwd);
		return ;
	}
	update_pwd(shell, "OLDPWD=", oldpwd);
	if (count == 1)
	{
		if (chdir(home) == -1)
			error_and_exit_code(shell);
	}
	else
		cd_to_arg(command, shell);
}
