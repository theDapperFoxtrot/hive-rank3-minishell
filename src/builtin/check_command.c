/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saylital <saylital@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:17:41 by saylital          #+#    #+#             */
/*   Updated: 2025/01/27 17:41:41:36 by saylital         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_builtin(char **command, t_ms *shell)
{
	if (ft_strncmp(command[0], "echo", 4) == 0)
	{
		ft_echo(command, shell);
		return (1);
	}
	else if (ft_strncmp(command[0], "pwd", 3) == 0)
	{
		ft_pwd(command, shell);
		return (1);
	}
	else if (ft_strncmp(command[0], "env", 3) == 0)
	{
		ft_env(command, shell);
		return (1);
	}
	else if (ft_strncmp(command[0], "cd", 2) == 0)
	{
		ft_cd(command, shell);
		return (1);
	}
	else if (ft_strncmp(command[0], "export", 6) == 0)
	{
		ft_export(command, shell);
		return (1);
	}
	else if (ft_strncmp(command[0], "unset", 5) == 0)
	{
		ft_unset(command, shell);
		return (1);
	}
	return (0);
}

void	fork_error(int *new_pipe)
{
	ft_putendl_fd("pipex: Fork error: forking failed", 2);
	close(new_pipe[0]);
	close(new_pipe[1]);
	exit(EXIT_FAILURE);
}

void	close_fds(int *new_pipe)
{
	close(new_pipe[0]);
	close(new_pipe[1]);
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
	ft_putstr_fd("find_directory error: ", 2);
	ft_putendl_fd(splitted_args, 2);
	return (NULL);
}

char	*find_executable_path(t_ms *shell)
{
	char	*get_path;
	char	**path_directory;
	char	*found_path;
	t_command	*command;
	char 	**envp;

	envp = shell->env_list;
	command = shell->commands;
		get_path = find_path(command->args[0], envp);
	find_path(command->args[0], envp);
	if (get_path == NULL)
		return (NULL);
	path_directory = ft_split(get_path, ':');
	if (path_directory == NULL)
		return (NULL);
	found_path = find_directory(path_directory, command->args[0]);
	return (found_path);
}

void handle_input_redirection(int *new_pipe, t_command *command)
{
	if (command->redir_in && command->heredoc)
		read_file(new_pipe, command);
	else if (command->redir_in)
		read_file(new_pipe, command);

}

void handle_output_redirection(int *new_pipe, t_command *command)
{
	if (command->redir_out)
		write_file(new_pipe, command);
	if (command->append_mode)
		append_file(new_pipe, command);

}

void check_command(t_ms *shell)
{
	t_command *command;
	int prev_pipe_in = -1;
	int new_pipe[2];

	command = shell->commands;
	while (command)
	{
		// Create pipe ONLY if there's a next command
		if (command->next && pipe(new_pipe) == -1)
		{
			ft_putendl_fd("minishell: pipe failed", 2);
			exit(EXIT_FAILURE);
		}

		if (ft_strncmp(command->args[0], "exit", 4) == 0)
			ft_exit(command->args, shell);

		command->pid = fork();
		if (command->pid == -1)
			fork_error(new_pipe);

		if (command->pid == 0) // CHILD PROCESS
		{
			// Handle input redirection from previous command
			if (prev_pipe_in != -1)
			{
				dup2(prev_pipe_in, STDIN_FILENO);
				close(prev_pipe_in);
			}

			// Handle output redirection to next command
			if (command->next)
			{
				dup2(new_pipe[1], STDOUT_FILENO);
				close(new_pipe[1]);
				close(new_pipe[0]);
			}

			// Handle file redirections (your existing logic)
			if (command->redir_in)
				handle_input_redirection(new_pipe, command); // You need to implement this
			if (command->redir_out)
				handle_output_redirection(new_pipe, command); // You need to implement this

			// Execute builtin or external command
			if (!is_builtin(command->args, shell))
			{
				char *path = find_executable_path(shell);
				if (!path)
					exit(127); // Command not found
				execve(path, command->args, shell->env_list);
				perror("minishell");
				exit(126); // Permission error
			}
			cleanup(shell);
			exit(shell->exit_code);
		}
		else // PARENT PROCESS
		{
			// Close previous pipe input if used
			if (prev_pipe_in != -1)
				close(prev_pipe_in);
			// Prepare for next command
			if (command->next)
			{
				close(new_pipe[1]); // Close write end, keep read end for next
				prev_pipe_in = new_pipe[0];
			}
			command = command->next;
		}
	}
	// Close final pipe input if any
	if (prev_pipe_in != -1)
		close(prev_pipe_in);
	// Wait for all children
	while (waitpid(-1, NULL, 0) > 0);
}

