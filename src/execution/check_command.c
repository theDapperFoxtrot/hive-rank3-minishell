#include "../../include/minishell.h"

int is_parent_builtin(char **command, t_ms *shell)
{
	if (ft_strncmp(command[0], "export", 6) == 0 && ft_strlen(command[0]) == 6)
	{
		if (shell->pipe_count > 0)
			return (0);
		ft_export(command, shell);
		return (1);
	}
	if (ft_strncmp(command[0], "unset", 5) == 0 && ft_strlen(command[0]) == 5)
	{
		ft_unset(command, shell);
		return (1);
	}
		else if (ft_strncmp(command[0], "cd", 2) == 0 && ft_strlen(command[0]) == 2)
	{
		ft_cd(command, shell);
		return (1);
	}
		else if (ft_strncmp(command[0], "pwd", 3) == 0 && ft_strlen(command[0]) == 3)
	{
		ft_pwd(command, shell);
		return (1);
	}

	return (0);
}

int	is_builtin(char **command, t_ms *shell)
{
	if (ft_strncmp(command[0], "export", 6) == 0 && ft_strlen(command[0]) == 6)
	{
		ft_export(command, shell);
		return (1);
	}
	else if (ft_strncmp(command[0], "echo", 4) == 0 && ft_strlen(command[0]) == 4)
	{
		ft_echo(command, shell);
		return (1);
	}
	else if (ft_strncmp(command[0], "env", 3) == 0 && ft_strlen(command[0]) == 3)
	{
		ft_env(command, shell);
		return (1);
	}
	return (0);
}

void	fork_error(int *new_pipe)
{
	ft_putendl_fd("minishell: Fork error: forking failed", 2);
	close(new_pipe[0]);
	close(new_pipe[1]);
	exit(EXIT_FAILURE);
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
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(cmd, 2);
		return (NULL);
	}
	return (path);
}

int	is_dir(char *str)
{
	int		fd3;

	fd3 = open(str, O_DIRECTORY);
	if (fd3 != -1)
	{
		close(fd3);
		return (1);
	}
	return (0);
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
			ft_putendl_fd("minishell: permission denied:", 2);
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

char	*find_executable_path(t_ms *shell, t_command *command)
{
	char	*get_path;
	char	**path_directory;
	char	*found_path;
	char 	**envp;

	if (ft_strchr(command->args[0], '/'))
	{
		if (access(command->args[0], F_OK) == 0)
		{
			if (access(command->args[0], X_OK) == 0)
			{
				if (!is_dir(command->args[0]))
					return (ft_strdup(command->args[0]));
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(command->args[0], 2);
				ft_putstr_fd(": Is a directory\n", 2);
				exit(126);
			}
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(command->args[0], 2);
			ft_putstr_fd(": Permission denied\n", 2);
			exit(126);
		}
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(command->args[0], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (NULL);
	}
	envp = shell->env_list;
		get_path = find_path(command->args[0], envp);
	find_path(command->args[0], envp);
	if (get_path == NULL)
		return (NULL);
	path_directory = ft_split(get_path, ':');
	if (path_directory == NULL)
		return (NULL);
	found_path = find_directory(path_directory, command->args[0]);
	free_split(path_directory);
	return (found_path);
}

void handle_input_redirection(t_ms *shell, t_command *command, char *symbol, char *file)
{
	int pipefd[2];
	if (ft_strncmp(symbol, "<<", 2) == 0)
	{
		if (pipe(pipefd) == -1)
        {
			perror("pipe");
            exit(EXIT_FAILURE);
        }
        command->pid = fork();
        if (command->pid == -1)
        {
			perror("fork");
			free(shell->commands->heredoc_line);
			cleanup(shell, 1);
            exit(EXIT_FAILURE);
        }
        if (command->pid == 0) // Child process
        {
			close(pipefd[0]); // Close read end
			if (write(pipefd[1], command->heredoc_line, ft_strlen(command->heredoc_line)) == -1)
			{
				ft_putendl_fd("minishell: write failed", 2);
				close(pipefd[0]);
				close(pipefd[1]);
				free(shell->commands->heredoc_line);
				cleanup(shell, 1);
				exit(EXIT_FAILURE);
			}
            close(pipefd[1]);
			free(shell->commands->heredoc_line);
			cleanup(shell, 1);
            exit(EXIT_SUCCESS);
        }
        else // Parent process
        {
			close(pipefd[1]); // Close write end
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
			if (command->redir_out)
			write_file(shell, file);
			while (waitpid(-1, NULL, 0) > 0);
        }
	}
	// read_heredoc(command);
	else if (ft_strncmp(symbol, "<", 1) == 0)
		read_file(shell, file);
}

void handle_output_redirection(t_ms *shell, t_command *command, char *symbol, char *file)
{
	(void) command;
	if (ft_strncmp(symbol, ">", 1) == 0)
			write_file(shell, file);
	else if (ft_strncmp(symbol, ">>", 2) == 0)
			append_file(shell, file);
}

void check_command(t_ms *shell, t_command *command)
{
	int			prev_pipe_in = -1;
	int			new_pipe[2];
	int			status;
	int			i;
	char		*path;

	shell->last_pid = 0;
	shell->child_count = 0;
	while (command)
	{
		if (command->args)
		{
			if (ft_strncmp(command->args[0], "exit", 4) == 0 && ft_strlen(command->args[0]) == 4)
			{
				ft_exit(command->args, shell);
				command = command->next;
				continue ;
			}
			if (is_parent_builtin(command->args, shell))
			{
				command = command->next;
				continue ;
			}
		}
		if (command->next && pipe(new_pipe) == -1)
		{
			ft_putendl_fd("minishell: pipe failed", 2);
			exit(EXIT_FAILURE);
		}
		shell->child_count++;
		command->pid = fork();
		sig_child(&sig_handler_child);
		if (command->pid == -1)
			fork_error(new_pipe);
		if (command->pid == 0)
		{
			if (prev_pipe_in != -1)
			{
				dup2(prev_pipe_in, STDIN_FILENO);
				close(prev_pipe_in);
			}
			if (command->next)
			{
				dup2(new_pipe[1], STDOUT_FILENO);
				close(new_pipe[1]);
				close(new_pipe[0]);
			}
			i = 0;
			while (command->command_input[i] && \
			(command->heredoc || command->redir_in ||\
			command->redir_out || command->append_mode))
			{
				if (ft_strncmp(command->command_input[i], "<", 1) == 0)
				{
					handle_input_redirection(shell, command, command->command_input[i], command->command_input[i + 1]);
				}
				if (ft_strncmp(command->command_input[i], "<<", 2) == 0)
					handle_input_redirection(shell, command, command->command_input[i], NULL);
				if (ft_strncmp(command->command_input[i], ">", 1) == 0)
				{
					handle_output_redirection(shell, command, command->command_input[i], command->command_input[i + 1]);
				}
				if (ft_strncmp(command->command_input[i], ">>", 2) == 0)
				{
					handle_output_redirection(shell, command, command->command_input[i], command->command_input[i + 1]);
				}
				i++;
			}
			if (command->args)
			{
				if (!is_builtin(command->args, shell))
				{
					path = find_executable_path(shell, command);
					if (!path)
					{
						cleanup(shell, 1);
						exit(127);
					}
					if (execve(path, command->args, shell->env_list) == -1)
					{
						perror("minishell");
						shell->exit_code = 126;
						exit(shell->exit_code);
					}
				}
			}
			cleanup(shell, 1);
			exit(shell->exit_code);
		}
		else
		{
			if (prev_pipe_in != -1)
				close(prev_pipe_in);
			if (!command->next)
				shell->last_pid = command->pid;
			if (command->next)
			{
				close(new_pipe[1]);
				prev_pipe_in = new_pipe[0];
			}
			command = command->next;
		}
	}
	if (prev_pipe_in != -1)
		close(prev_pipe_in);
	// wait for all children to finish IN ORDER
	while (shell->child_count-- > 0)
	{
		shell->wpid = waitpid(-1, &status, 0);
		if (shell->wpid == shell->last_pid)
			shell->exit_code = WEXITSTATUS(status);
	}
}
