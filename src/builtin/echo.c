/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 10:24:22 by saylital          #+#    #+#             */
/*   Updated: 2025/03/25 15:41:32 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_n(char *command)
{
	int	i;

	i = 1;
	while (command[i])
	{
		if (command[i] != 'n')
			return (0);
		i++;
	}
	return (1);
	i = 1;
	while (command[i])
	{
		if (command[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	ft_echo(char **command, t_ms *shell)
void	ft_echo(char **command, t_ms *shell)
{
	int	i;
	int	no_newline;
	int	i;
	int	no_newline;

	i = 1;
	no_newline = 1;
	while (ft_strncmp(command[i], "-n", 2) == 0 && check_n(command[i]))
	{
		no_newline = 0;
		i++;
	}
	while (command[i])
	{
		ft_putstr_fd(command[i++], 1);
		if (command[i])
			ft_putstr_fd(" ", 1);
	}
	if (no_newline)
		ft_putstr_fd("\n", 1);
	shell->exit_code = 0;
	return ;
	i = 1;
	no_newline = 1;
	while (ft_strncmp(command[i], "-n", 2) == 0 && check_n(command[i]))
	{
		no_newline = 0;
		i++;
	}
	while (command[i])
	{
		ft_putstr_fd(command[i++], 1);
		if (command[i])
			ft_putstr_fd(" ", 1);
	}
	if (no_newline)
		ft_putstr_fd("\n", 1);
	shell->exit_code = 0;
	return ;
}

