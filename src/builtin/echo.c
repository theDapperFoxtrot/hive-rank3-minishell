/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 10:24:22 by saylital          #+#    #+#             */
/*   Updated: 2025/03/11 16:46:37 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_echo(char **command, t_ms *shell)
{
	int	i;
	int	no_newline;

	i = 1;
	no_newline = 1;
	if ((ft_strncmp(command[1], "-", 1 == 0) && ft_strncmp(command[2], "n", 1) == 0))
	{
		no_newline = 0;
		i += 2;
	}
	else if (ft_strncmp(command[1], "-n", 2) == 0 && ft_strlen(command[1]) == 2)
	{
		no_newline = 0;
		i++;
	}
	while (command[i])
	{
		ft_putstr_fd(command[i++], 1);
		if (command[i] != NULL)
			ft_putstr_fd(" ", 1);
	}
	if (no_newline)
		ft_putstr_fd("\n", 1);
	shell->exit_code = 0;
	return ;
}
