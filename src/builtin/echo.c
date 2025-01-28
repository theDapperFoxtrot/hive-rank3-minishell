/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 10:24:22 by saylital          #+#    #+#             */
/*   Updated: 2025/01/28 15:16:11 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// void	ft_echo(char **command, t_ms *shell)
// {
// 	int	i;

// 	shell->commands->not_builtin = 0;
// 	i = 1;
// 	if (ft_strncmp(command[1], "$?", 2) == 0)
// 	{
// 		printf("%d\n", shell->exit_code);
// 		return ;
// 	}
// 	if (ft_strncmp(command[1], "-n", 2) == 0)
// 		i++;
// 	while (command[i])
// 	{
// 		printf("%s", command[i++]);
// 		if (command[i] != NULL)
// 			printf(" ");
// 	}
// 	if (ft_strncmp(command[1], "-n", 2) != 0)
// 		printf("\n");
// 	shell->exit_code = 0;
// 	return ;
// }
void ft_echo(char **command, t_ms *shell)
{
    int i;
    shell->commands->not_builtin = 0;
    i = 1;

    if (command[1] && ft_strncmp(command[1], "$?", 2) == 0)
    {
        ft_putnbr_fd(shell->exit_code, STDOUT_FILENO);
        ft_putchar_fd('\n', STDOUT_FILENO);
        return;
    }

    if (command[1] && ft_strncmp(command[1], "-n", 2) == 0)
        i++;

    while (command[i])
    {
        ft_putstr_fd(command[i], STDOUT_FILENO);
        i++;
        if (command[i] != NULL)
            ft_putchar_fd(' ', STDOUT_FILENO);
    }

    if (!command[1] || ft_strncmp(command[1], "-n", 2) != 0)
        ft_putchar_fd('\n', STDOUT_FILENO);

    shell->exit_code = 0;
    return;
}