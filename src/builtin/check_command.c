/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:17:41 by saylital          #+#    #+#             */
/*   Updated: 2025/01/22 17:35:09 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	is_builtin(char **command, t_ms *shell)
{
	if (ft_strncmp(command[0], "echo", 4) == 0)
		ft_echo(command, shell);
	else if (ft_strncmp(command[0], "pwd", 3) == 0)
		ft_pwd(command, shell);
	else if (ft_strncmp(command[0], "exit", 4) == 0)
		ft_exit(command, shell);
	else if (ft_strncmp(command[0], "env", 3) == 0)
		ft_env(command, shell);
	else if (ft_strncmp(command[0], "cd", 2) == 0)
		ft_cd(command, shell);
	else if (ft_strncmp(command[0], "export", 6) == 0)
		ft_export(command, shell);
	else if (ft_strncmp(command[0], "unset", 5) == 0)
		ft_unset(command, shell);
}

// void	does_not_exist(char **command, t_ms *shell)
// {
// 	return ;
// }

void	check_command(t_ms *shell)
{
	t_command	*command;

	command = shell->commands;
	while (command)
	{
		is_builtin(command->args, shell);
		// if (is_in_env(command, shell))
		// 	return ;
		// else
			// does_not_exist(command, shell);
		if (command->next)
			command = command->next;
		else
			break ;
	}
}
