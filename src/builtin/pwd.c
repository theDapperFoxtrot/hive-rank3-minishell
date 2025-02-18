/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:50:38 by saylital          #+#    #+#             */
/*   Updated: 2025/02/18 16:45:03 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_pwd(char **command, t_ms *shell)
{
	char	*pwd;
	int		i;

	(void) command;
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		i = 0;
		while (shell->env_list[i])
		{
			if (ft_strncmp(shell->env_list[i], "PWD=", 4) == 0)
			{
				ft_putstr_fd(shell->env_list[i], 1);
				ft_putstr_fd("\n", 1);
				return ;
			}
			i++;
		}
	}
	i = 0;
		while (shell->env_list[i])
		{
			if (ft_strncmp(shell->env_list[i], "PWD=", 4) == 0)
			{
				free(shell->env_list[i]);
				shell->env_list[i] = ft_strjoin("PWD=", pwd);
				break ;
			}
			i++;
		}
	ft_putstr_fd(pwd, 1);
	ft_putstr_fd("\n", 1);
	free(pwd);
	shell->exit_code = 0;
	return ;
}
