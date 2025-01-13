/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 10:24:22 by saylital          #+#    #+#             */
/*   Updated: 2025/01/13 19:03:10 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_echo(char **command, t_ms *shell)
{
	int	i;

	i = 1;
	if (ft_strncmp(command[1], "$?", 2) == 0)
	{
		printf("%d\n", shell->exit_code);
		return ;
	}
	if (ft_strncmp(command[1], "-n", 2) == 0)
		i++;
	while (command[i])
	{
		printf("%s", command[i++]);
		if (command[i] != NULL)
			printf(" ");
	}
	if (ft_strncmp(command[1], "-n", 2) != 0)
		printf("\n");
	shell->exit_code = 0;
	return ;
}