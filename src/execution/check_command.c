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
	else if ((ft_strncmp(command[0], "echo", 4) == 0 && ft_strlen(command[0]) == 4) || (ft_strncmp(command[0], "/bin/echo", 9) == 0 && ft_strlen(command[0]) == 9))
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

void	find_exec_path_error(t_ms *shell, t_command *command, char *message, int exit_code)
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
	char 	**envp;

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

void	hir_hd_child_process(t_ms *shell, t_command *command, int *pipefd)
{
	close(pipefd[0]);
	if (write(pipefd[1], command->heredoc_line, ft_strlen(command->heredoc_line)) == -1)
	{
		ft_putendl_fd("minishell: write failed", 2);
		close(pipefd[1]);
		cleanup(shell, 1);
		exit(EXIT_FAILURE);
	}
	close(pipefd[1]);
	cleanup(shell, 1);
	exit(EXIT_SUCCESS);
}

void handle_input_redirection(t_ms *shell, t_command *command, char *symbol, char *file)
{
	int pipefd[2];

	if (ft_strncmp(symbol, "<<", 2) == 0)
	{
		if (pipe(pipefd) == -1)
			error_free_clean_exit(shell, "pipe");
		command->pid = fork();
		if (command->pid == -1)
			error_free_clean_exit(shell, "fork");
        if (command->pid == 0)
			hir_hd_child_process(shell, command, pipefd);
		else
        {
			close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
			// printf("command->command_input[2]: %s\n", command->command_input[2]);
			if (command->redir_out)
				write_file(shell, command->command_input[2]);
			while (waitpid(-1, NULL, 0) > 0);
        }
	}
	else if (ft_strncmp(symbol, "<", 1) == 0)
		read_file(shell, file);
}

void handle_output_redirection(t_ms *shell, char *symbol, char *file)
{
	if (ft_strncmp(symbol, ">>", 2) == 0)
			append_file(shell, file);
	else if (ft_strncmp(symbol, ">", 1) == 0)
			write_file(shell, file);
}

void	wait_for_kids(t_ms *shell)
{
	while (shell->child_count-- > 0)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		shell->wpid = waitpid(-1, &shell->status, 0);
		if (shell->wpid == shell->last_pid)
		{
			if (WIFSIGNALED(shell->status))
			{
				if (WTERMSIG(shell->status) == SIGINT)
					write(1, "\n", 1);
				else if (WTERMSIG(shell->status) == SIGQUIT)
					ft_putstr_fd("Quit (core dumped)\n", 2);
				shell->exit_code = 128 + WTERMSIG(shell->status);
			}
			else
				shell->exit_code = WEXITSTATUS(shell->status);
		}
	}
}

void	execute_redir(t_ms *shell, t_command *command, int i)
{
	if (ft_strncmp(command->command_input[i], ">>", 2) == 0)
		handle_output_redirection(shell, command->command_input[i], command->command_input[i + 1]);
	if (ft_strncmp(command->command_input[i], "<<", 2) == 0)
		handle_input_redirection(shell, command, command->command_input[i], command->command_input[i + 1]);
	if (ft_strncmp(command->command_input[i], ">", 1) == 0)
		handle_output_redirection(shell, command->command_input[i], command->command_input[i + 1]);
	if (ft_strncmp(command->command_input[i], "<", 1) == 0)
		handle_input_redirection(shell, command, command->command_input[i], command->command_input[i + 1]);
}

void	execute_command(t_ms *shell, t_command *command, int exec)
{
	char	*path;

	if (!exec)
	{
		path = find_executable_path(shell, command);
		if (!path)
		{
			cleanup(shell, 1);
			exit(127);
		}
		if (execve(path, command->args, shell->env_list) == -1)
		{
			if (g_signal == SIGINT)
				write(1, "\n", 1);
			perror("minishell");
			shell->exit_code = 126;
			cleanup(shell, 1);
			exit(shell->exit_code);
		}
	}
}

void print_cmd_input(char **command)
{
	int i;

	i = 0;
	ft_putstr_fd("Command input: ", 1);
	while (command[i])
	{
		ft_putstr_fd(command[i], 1);
		ft_putstr_fd(", ", 1);
		i++;
	}
	ft_putchar_fd('\n', 1);
}

void	child_process(t_ms *shell, t_command *command, int *prev_pipe_in, int *new_pipe)
{
	int			exec;
	int			i;

	if (default_signals() == 1)
		print_error("Error: default_signals failed", shell, 1, 1);
	check_signals(SIGINT, &sig_handler_child);
	check_signals(SIGQUIT, &sig_handler_child);
	if (*prev_pipe_in != -1)
	{
		dup2(*prev_pipe_in, STDIN_FILENO);
		close(*prev_pipe_in);
	}
	if (command->next)
	{
		close(new_pipe[0]);
		dup2(new_pipe[1], STDOUT_FILENO);
		close(new_pipe[1]);
	}
	i = -1;
	while (command->command_input[++i])
		execute_redir(shell, command, i);
	if (command->args)
		exec = is_builtin(command->args, shell);
	if (command->args)
		execute_command(shell, command, exec);
	cleanup(shell, 1);
	exit(shell->exit_code);
}

t_command	*parent_process(t_ms *shell, t_command *command, int *prev_pipe_in, int *new_pipe)
{
	if ((*prev_pipe_in) != -1)
		close((*prev_pipe_in));
	if (!command->next)
		shell->last_pid = command->pid;
	if (command->next)
	{
		close(new_pipe[1]);
		(*prev_pipe_in) = new_pipe[0];
	}
	command = command->next;
	return (command);
}

t_command *check_for_dots(t_command *command)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(command->args[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	command = command->next;
	return (command);
}

t_command *check_for_exit(t_ms *shell, t_command *command, int *new_pipe)
{
	if (shell->child_count > 1)
		close(new_pipe[0]);
	ft_exit(command, shell);
	command = command->next;
	return (command);
}

t_command *checking_for_select_commands(t_ms *shell, t_command *command, int *new_pipe)
{
	if ((ft_strncmp(command->args[0], "..", 2) == 0 && ft_strlen(command->args[0]) == 2))
	{
		command = check_for_dots(command);
		shell->select_command_found = 1;
		return (command);
	}
	if (ft_strncmp(command->args[0], "exit", 4) == 0 && ft_strlen(command->args[0]) == 4)
	{
		command = check_for_exit(shell, command, new_pipe);
		shell->select_command_found = 1;
		return (command);
	}
	if (is_parent_builtin(command->args, shell))
	{
		command = command->next;
		shell->select_command_found = 1;
	}
	return (command);
}

void 	pipe_failure(t_ms *shell)
{
	ft_putendl_fd("minishell: pipe failed", 2);
	cleanup(shell, 1);
	exit(EXIT_FAILURE);
}

void check_command(t_ms *shell, t_command *command)
{
	int			prev_pipe_in;
	int			new_pipe[2];

	prev_pipe_in = -1;
	shell->last_pid = 0;
	shell->child_count = 0;
	shell->select_command_found = 0;
	while (command)
	{
		if (command->args)
		{
			command = checking_for_select_commands(shell, command, new_pipe);
			if (shell->select_command_found)
			{
				shell->select_command_found = 0;
				continue ;
			}
		}
		if (command->next && pipe(new_pipe) == -1)
			pipe_failure(shell);
		shell->child_count++;
		command->pid = fork();
		if (command->pid == -1)
			fork_error(new_pipe);
		if (command->pid == 0)
			child_process(shell, command, &prev_pipe_in, new_pipe);
		else
			command = parent_process(shell, command, &prev_pipe_in, new_pipe);
	}
	if (prev_pipe_in != -1)
		close(prev_pipe_in);
	wait_for_kids(shell);
}

