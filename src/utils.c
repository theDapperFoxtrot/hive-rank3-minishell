/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saylital <saylital@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:39:22 by saylital          #+#    #+#             */
/*   Updated: 2025/03/19 19:03:53 by saylital         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_args(char **commands)
{
	int	i;

	i = 0;
	if (commands)
	{
		while (commands[i])
		{
			free(commands[i]);
			i++;
		}
	}
	free(commands);
}

void	free_env(t_ms *shell)
{
	int	i;

	i = 0;
	if (shell->env_list)
	{
		while (shell->env_list[i])
		{
			free(shell->env_list[i]);
			i++;
		}
	}
	free(shell->env_list);
}

void	print_error(char *message, t_ms *shell, \
					unsigned char status, int clean_shell)
{
	ft_putendl_fd(message, 2);
	cleanup(shell, clean_shell);
	shell->exit_code = status;
}

int	count_args(char **command)
{
	int	i;

	i = 0;
	while (command[i])
	{
		i++;
	}
	return (i);
}

void	malloc_error(t_ms *shell)
{
	print_error("Error: malloc failed", shell, 1, 1);
	exit(shell->exit_code);
}
