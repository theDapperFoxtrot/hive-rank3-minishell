/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:17:41 by saylital          #+#    #+#             */
/*   Updated: 2025/01/27 16:34:19 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	is_builtin(char **command, t_ms *shell)
{
	if (ft_strncmp(command[0], "echo", 4) == 0)
		ft_echo(command, shell);
	else if (ft_strncmp(command[0], "pwd", 3) == 0)
		ft_pwd(command, shell);
	else if (ft_strncmp(command[0], "env", 3) == 0)
		ft_env(command, shell);
	else if (ft_strncmp(command[0], "cd", 2) == 0)
		ft_cd(command, shell);
	else if (ft_strncmp(command[0], "export", 6) == 0)
		ft_export(command, shell);
	else if (ft_strncmp(command[0], "unset", 5) == 0)
		ft_unset(command, shell);
}

void	fork_error(t_ms *shell)
{
	ft_putendl_fd("pipex: Fork error: forking failed", 2);
	close(shell->fd[0]);
	close(shell->fd[1]);
	exit(EXIT_FAILURE);
}

void	close_fds(t_ms *shell)
{
	close(shell->fd[0]);
	close(shell->fd[1]);
}

int	handle_exit(t_ms *shell)
{
	int		pipe_count;
	int		status;
	int		exit_code;
	int		child_pid;

	while (shell->commands)
	{
		shell->commands = shell->commands->next;
	}
	shell->i = 0;
	pipe_count = shell->pipe_count;
	while (shell->i < pipe_count + 1)
	{
		child_pid = waitpid(-1, &status, 0);
		if (WIFEXITED(status) && child_pid == shell->commands->pid)
			exit_code = WEXITSTATUS(status);
		shell->i++;
	}
	return (exit_code);
}

char	*find_path(char *cmd, char **envp)
{
	char	*path;
	int		i;

	i = 0;
	path = NULL;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path = envp[i] + 5;
			break ;
		}
		i++;
	}
	if (path == NULL)
	{
		ft_putstr_fd("pipex: command not found: ", 2);
		ft_putendl_fd(cmd, 2);
		return (NULL);
	}
	return (path);
}

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
			ft_putendl_fd("pipex: permission denied:", 2);
			free(executable_path);
			exit(126);
		}
		free(executable_path);
		i++;
	}
	ft_putstr_fd("pipex: command not found: ", 2);
	ft_putendl_fd(splitted_args, 2);
	return (NULL);
}

char	*find_executable_path(t_ms *shell)
{
	char	*get_path;
	char	**path_directory;
	char	*found_path;
	char	*command;
	char 	**envp;

	envp = shell->env_list;
	command = shell->commands->args[0];
	get_path = find_path(command, envp);
	if (get_path == NULL)
		return (NULL);
	path_directory = ft_split(get_path, ':');
	if (path_directory == NULL)
		return (NULL);
	found_path = find_directory(path_directory, command);
	// free(command);
	// free_split(path_directory);
	return (found_path);
}


void check_command(t_ms *shell)
{
	t_command *command;
	char		*path;

	command = shell->commands;
	while (command)
	{
		if (pipe(shell->fd) == -1)
		{
			ft_putendl_fd("minishell: pipe failed", 2);
			exit(EXIT_FAILURE);
		}
		if (ft_strncmp(command->args[0], "exit", 4) == 0)
			ft_exit(command->args, shell);
		command->pid = fork();
		if (command->pid == -1)
		{
			ft_putendl_fd("minishell: fork failed", 2);
			ft_exit(command->args, shell);
		}
		if (command->pid == 0)
		{
			if (command->redir_in && command->heredoc)
				read_file(shell->fd, command);
			else if (command->redir_in)
				read_file(shell->fd, command);
			if (command->redir_out)
				write_file(shell->fd, command);
			if (command->append_mode)
				append_file(shell->fd, command);
			is_builtin(command->args, shell);
			if (command->not_builtin)
			{
				path = find_executable_path(shell);
				if (execve(path, command->args, shell->env_list) == -1)
				{
					ft_putstr_fd("minishell: ", 2);
					perror(command->args[0]);
					ft_exit(command->args, shell);
				}
			}
			ft_exit(command->args, shell);
		}
		// else
		// {
		// 	close(shell->fd[0]);
		// 	close(shell->fd[1]);
		// }
		command = command->next;
	}
	close(shell->fd[0]);
	close(shell->fd[1]);
	while (wait(NULL) > 0);
}
