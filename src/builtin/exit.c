/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:33:02 by saylital          #+#    #+#             */
/*   Updated: 2025/01/13 19:03:16 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	check_numeric(char **command, t_ms *shell)
{
	int	i;

	i = 0;
	if (!command[1])
		return ;
	if (command[1][i] == '#')
	{
		shell->exit_code = 0;
		printf("exit\n");
		free_args(command);
		free_env(shell);
		exit(shell->exit_code);
	}
	while(command[1][i])
	{
		if (ft_isalpha(command[1][i]) == 1)
		{
			print_error("exit: numeric argument required", shell, 2);
			exit(shell->exit_code);
		}
		i++;	
	}
}

void	ft_exit(char **command, t_ms *shell)
{
	if (!command[1])
	{
		printf("exit\n");
		free_args(command);
		free_env(shell);
		exit(shell->exit_code);		
	}
	check_numeric(command, shell);
	if (command[2])
	{
		print_error("exit: too many arguments", shell, 1);
		return ;
	}
	else if (command[1])
		shell->exit_code = ft_atoi(command[1]);
	printf("exit\n");
	free_args(command);
	free_env(shell);
	exit(shell->exit_code);
}
