/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_command2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 20:08:48 by smishos           #+#    #+#             */
/*   Updated: 2025/03/19 20:08:49 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*find_directory(char **dir, char *splitted_args)
{
	int		i;
	char	*executable_path;
	char	*slash;

	i = 0;
	while (dir[i] != NULL)
	{
		slash = ft_strjoin(dir[i], "/");
		executable_path = ft_strjoin(slash, splitted_args);
		free(slash);
		if (access(executable_path, F_OK) == 0)
		{
			if (access(executable_path, X_OK) == 0)
				return (executable_path);
			ft_putendl_fd("minishell: Permission denied:", 2);
			free(executable_path);
			exit(126);
		}
		free(executable_path);
		i++;
	}
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(splitted_args, 2);
	ft_putstr_fd(": command not found\n", 2);
	return (NULL);
}

void	find_exec_path_error(t_ms *shell, t_command *command, \
							char *message, int exit_code)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(command->args[0], 2);
	ft_putstr_fd(message, 2);
	if (exit_code)
	{
		cleanup(shell, 1);
		exit(exit_code);
	}
}

char	*access_check(t_ms *shell, t_command *command)
{
	if (access(command->args[0], F_OK) == 0)
	{
		if (access(command->args[0], X_OK) == 0)
		{
			if (!is_dir(command->args[0]))
				return (ft_strdup(command->args[0]));
			find_exec_path_error(shell, command, ": Is a directory\n", 126);
		}
		find_exec_path_error(shell, command, ": Permission denied\n", 126);
	}
	find_exec_path_error(shell, command, ": No such file or directory\n", 0);
	return (NULL);
}

char	*find_executable_path(t_ms *shell, t_command *command)
{
	char	*get_path;
	char	**path_directory;
	char	*found_path;
	char	**envp;

	if (ft_strchr(command->args[0], '/'))
		return (access_check(shell, command));
	envp = shell->env_list;
	get_path = find_path(command->args[0], envp);
	if (get_path == NULL)
		return (NULL);
	path_directory = ft_split(get_path, ':');
	if (path_directory == NULL)
		return (NULL);
	found_path = find_directory(path_directory, command->args[0]);
	free_split(path_directory);
	return (found_path);
}

void	error_free_clean_exit(t_ms *shell, char *message)
{
	perror(message);
	free(shell->commands->heredoc_line);
	cleanup(shell, 1);
	exit(EXIT_FAILURE);
}
