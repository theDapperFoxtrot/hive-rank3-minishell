/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_pwd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 13:47:21 by saylital          #+#    #+#             */
/*   Updated: 2025/02/03 16:26:43 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	update_pwd(t_ms *shell, char *string, char *value)
{
	int		i;
	int		len;
	char	*new_value;

	i = 0;
	new_value = ft_strjoin(string, value);
	free(value);
	if (!new_value)
	{
		print_error("Malloc failed update_env", shell, 1, 1);
		exit(shell->exit_code);
	}
	len = ft_strlen(string);
	while (shell->env_list[i])
	{
		if (ft_strncmp(shell->env_list[i], string, len) == 0)
		{
			free(shell->env_list[i]);
			shell->env_list[i] = new_value;
			return (0);
		}
		i++;
	}
	return (0);
}

void	error_and_exit_code(t_ms *shell)
{
	ft_putstr_fd("minishell: cd: HOME not set", 2);
	shell->exit_code = 1;
}

void if_count_is_1(t_ms *shell, char *oldpwd, char *home)
{
	if (chdir(home) == -1)
		error_and_exit_code(shell);
	free(oldpwd);
}

void	if_pipe_count(t_ms *shell, char **command, char *oldpwd)
{
	char *temp;

	temp = oldpwd;
	if (chdir(command[1]) == -1)
	{
		ft_putstr_fd("cd: no such file or directory: ", 2);
		ft_putendl_fd(command[1], 2);
		shell->exit_code = 1;
	}
	else
		chdir(oldpwd);
	free(oldpwd);
}
