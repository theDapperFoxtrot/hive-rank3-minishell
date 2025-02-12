/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:50:38 by saylital          #+#    #+#             */
/*   Updated: 2025/02/12 18:28:30 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_pwd(char **command, t_ms *shell)
{
	char	*pwd;

	(void) command;
	// if (command[1])
	// {
	// 	print_error("pwd: too many arguments", shell, 1, 0);
	// 	return ;
	// }
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		print_error("getcwd failed", shell, 1, 1);
		exit(shell->exit_code);
	}
	ft_putstr_fd(pwd, 1);
	ft_putstr_fd("\n", 1);
	// printf("%s\n", pwd);
	free(pwd);
	shell->exit_code = 0;
	return ;
}
