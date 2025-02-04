#include "../../include/minishell.h"

int is_parent_builtin(char **command, t_ms *shell)
{
	if (ft_strncmp(command[0], "cd", 2) == 0)
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
	return (0);
}

void	fork_error(int *new_pipe)
{
	ft_putendl_fd("pipex: Fork error: forking failed", 2);
	close(new_pipe[0]);
	close(new_pipe[1]);
	exit(EXIT_FAILURE);
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
	ft_putstr_fd("command not found: ", 2);
	ft_putendl_fd(splitted_args, 2);
	return (NULL);
}

char	*find_executable_path(t_ms *shell, t_command *command)
{
	char	*get_path;
	char	**path_directory;
	char	*found_path;
	char 	**envp;

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

void handle_input_redirection(t_ms *shell, t_command *command)
{
	int pipefd[2];

	if (command->heredoc)
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
				write_file(command);
			while (waitpid(-1, NULL, 0) > 0);
        }
	}
		// read_heredoc(command);
	else if (command->redir_in)
		read_file(command);
}

void handle_output_redirection(t_command *command)
{
	if (command->redir_out)
		write_file(command);
	if (command->append_mode)
		append_file(command);
}

void check_command(t_ms *shell)
{
	t_command *command;
	int prev_pipe_in = -1;
	int new_pipe[2];

	command = shell->commands;
	while (command)
	{
		if (ft_strncmp(command->args[0], "exit", 4) == 0)
			ft_exit(command->args, shell);
		if (command->next && pipe(new_pipe) == -1)
		{
			ft_putendl_fd("minishell: pipe failed", 2);
			exit(EXIT_FAILURE);
		}

		command->pid = fork();
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
			if (command->redir_in || command->heredoc)
				handle_input_redirection(shell, command);
			if (command->redir_out || command->append_mode)
				handle_output_redirection(command);
			if (is_parent_builtin(command->args, shell))
				command = command->next;
			else if (!is_builtin(command->args, shell))
			{
				char *path = find_executable_path(shell, command);
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
			cleanup(shell, 1);
			exit(shell->exit_code);
		}
		else
		{
			if (prev_pipe_in != -1)
				close(prev_pipe_in);
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
	while (waitpid(-1, NULL, 0) > 0);
	//get the exit code also from the child process
}


// int is_builtin(char **cmd)
// {
//     if (!ft_strcmp(cmd[0], "echo")) return ECHO;
//     if (!ft_strcmp(cmd[0], "cd"))   return CD;
//     if (!ft_strcmp(cmd[0], "export")) return EXPORT;
//     // ...
//     return NOT_A_BUILTIN;
// }

// void execute_builtin(int builtin_type, char **cmd, t_ms *shell)
// {
//     if (builtin_type == EXPORT)
//         ft_export(cmd, shell);
//     else if (builtin_type == CD)
//         ft_cd(cmd, shell);
//     // ...
// }

// void check_command(t_ms *shell)
// {
//     t_command *command = shell->commands;

//     while (command)
//     {
//         int type = is_builtin(command->args);
//         // If "export", "cd", "unset", etc.—run in the parent:
//         if (type == EXPORT || type == CD || type == UNSET || type == EXIT /* etc. */)
//         {
//             execute_builtin(type, command->args, shell);
//         }
//         else
//         {
//             // fork/exec for external commands *or* builtins safe in child (e.g. echo).
//             pid_t pid = fork();
//             if (pid == 0) {
//                 // child
//                 // redirect, pipes, etc...
//                 if (type != NOT_A_BUILTIN)
//                     execute_builtin(type, command->args, shell);
//                 else
//                     execve(...);
//                 exit(...);
//             } else {
//                 // parent
//                 waitpid(pid, &status, 0);
//             }
//         }
//         command = command->next;
//     }
// }
