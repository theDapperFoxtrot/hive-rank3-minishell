/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:50:38 by saylital          #+#    #+#             */
/*   Updated: 2025/01/28 15:50:50 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_pwd(char **command, t_ms *shell)
{
	char	*pwd;

	
	if (command[1])
	{
		print_error("pwd: too many arguments", shell, 1);
		return ;
	}
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		print_error("getcwd failed", shell, 1);
		exit(shell->exit_code);
	}
	printf("%s\n", pwd);
	free(pwd);
	shell->exit_code = 0;
	return ;
}
