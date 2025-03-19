/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saylital <saylital@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 10:24:22 by saylital          #+#    #+#             */
/*   Updated: 2025/03/19 17:36:03 by saylital         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// int check_n(char *command)
// {
// 	int i;

// 	if (!command)
// 		return (0);
// 	i = 1;
// 	while (command[i])
// 	{
// 		if (command[i] != 'n')
// 			return (0);
// 		i++;
// 	}
// 	return (1);
// }
// void	printing_echo(char **split, t_ms *shell, int i, int no_newline)
// {
// 	while (split[i])
// 	{
// 		ft_putstr_fd(split[i++], 1);
// 		if (split[i])
// 			ft_putstr_fd(" ", 1);
// 	}
// 	if (no_newline)
// 		ft_putstr_fd("\n", 1);
// 	shell->exit_code = 0;
// }

// void	ft_echo(char **command, t_ms *shell)
// {
// 	int	i;
// 	int	no_newline;
// 	char **split;
// 	int splitted;

// 	splitted = 0;
// 	i = 1;
// 	no_newline = 1;
// 	split = NULL;
// 	if (ft_strchr(command[i], ' '))
// 	{
// 		split = ft_split(command[i], ' ');
// 		splitted = 1;
// 		i = 0;
// 	}
// 	else
// 		split = command;
// 	while ((ft_strncmp(split[i], "-n", 2) == 0) && check_n(split[i]))
// 	{
// 		no_newline = 0;
// 		i++;
// 	}
// 	printing_echo(split, shell, i, no_newline);
// 	if (splitted)
// 		free_split(split);
// }

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
}

void	ft_echo(char **command, t_ms *shell)
{
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
}
