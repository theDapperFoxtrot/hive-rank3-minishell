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

int	if_not_pwd(t_ms *shell)
{
	int i;
	char	*read_here;

	i = 0;
	while (shell->env_list[i])
	{
		if (ft_strncmp(shell->env_list[i], "PWD=", 4) == 0)
		{
			read_here = ft_strchr(shell->env_list[i], '=');
			read_here++;
			ft_putstr_fd(read_here, 1);
			ft_putstr_fd("\n", 1);
			return (1);
		}
		i++;
	}
	return (0);
}

void	if_pwd(t_ms *shell, char *pwd)
{
	int		i;

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
}

void	ft_pwd(t_ms *shell)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		if (if_not_pwd(shell))
			return ;
	}
	if_pwd(shell, pwd);
	ft_putstr_fd(pwd, 1);
	ft_putstr_fd("\n", 1);
	free(pwd);
	shell->exit_code = 0;
	return ;
}
