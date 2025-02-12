/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 10:24:22 by saylital          #+#    #+#             */
/*   Updated: 2025/02/12 18:31:04 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_echo(char **command, t_ms *shell)
{
	int	i;

	
	i = 1;
	if (ft_strncmp(command[1], "$?", 2) == 0)
	{
		ft_putstr_fd(ft_itoa(shell->exit_code), 1);
		// printf("%d\n", shell->exit_code);
		// return ;
	}
	if (ft_strncmp(command[1], "-n", 2) == 0)
		i++;
	while (command[i])
	{
		ft_putstr_fd(command[i++], 1);
		// printf("%s", command[i++]);
		if (command[i] != NULL)
			ft_putstr_fd(" ", 1);
			// printf(" ");
	}
	if (ft_strncmp(command[1], "-n", 2) != 0)
		ft_putstr_fd("\n", 1);
		// printf("\n");
	shell->exit_code = 0;
	return ;
}
