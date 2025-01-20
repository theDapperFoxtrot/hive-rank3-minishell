/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saylital <saylital@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:33:02 by saylital          #+#    #+#             */
/*   Updated: 2025/01/20 15:31:56 by saylital         ###   ########.fr       */
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
		cleanup(shell);
		exit(shell->exit_code);
	}
	while (command[1][i])
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
		cleanup(shell);
		exit(shell->exit_code);
	}
	check_numeric(command, shell);
	if (command[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		shell->exit_code = 1;
		return ;
	}
	else if (command[1])
		shell->exit_code = ft_atoi(command[1]);
	printf("exit\n");
	cleanup(shell);
	exit(shell->exit_code);
}
