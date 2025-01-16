/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:17:41 by saylital          #+#    #+#             */
/*   Updated: 2025/01/16 19:09:37 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int is_builtin(char **command, t_ms *shell)
{
	if (ft_strncmp(command[0], "echo", 4) == 0)
	{
		ft_echo(command, shell);
		return (1);
	}
	else if (ft_strncmp(command[0], "pwd", 3) == 0)
	{
		ft_pwd(command, shell);
		return (1);
	}
	else if (ft_strncmp(command[0], "exit", 4) == 0)
	{
		ft_exit(command, shell);
		return (1);
	}
	else if (ft_strncmp(command[0], "env", 3) == 0)
	{
		ft_env(command, shell);
		return (1);
	}
	else if (ft_strncmp(command[0], "cd", 2) == 0)
	{
		ft_cd(command, shell);
		return (1);
	}
	else if (ft_strncmp(command[0], "export", 6) == 0)
	{
		ft_export(command, shell);
		return (1);
	}
	else
		return (0);
}


void	check_command(t_ms *shell)
{
	char	**command;

	command = shell->commands->args;
	while (command)
	{
		if (is_builtin(command, shell))
			return ;
		// else if (is_in_env(command, shell))
		// 	return ;
		else
			return ;
			// does_not_exist(command, shell);
		command = shell->commands->next->args;
	}
}
