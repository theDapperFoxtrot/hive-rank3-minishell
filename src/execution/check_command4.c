#include "../../include/minishell.h"

void	execute_command(t_ms *shell, t_command *command)
{
	char	*path;

	if (!shell->exec)
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

void	child_process(t_ms *shell, t_command *command, int *new_pipe)
{
	int			i;

	if (default_signals() == 1)
		print_error("Error: default_signals failed", shell, 1, 1);
	start_sig_checkers(&sig_handler_child);
	if (shell->prev_pipe_in != -1)
	{
		dup2(shell->prev_pipe_in, STDIN_FILENO);
		close(shell->prev_pipe_in);
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
		shell->exec = is_builtin(command->args, shell);
	if (command->args)
		execute_command(shell, command);
	cleanup(shell, 1);
	exit(shell->exit_code);
}

t_command	*parent_process(t_ms *shell, t_command *command, int *new_pipe)
{
	if ((shell->prev_pipe_in) != -1)
		close((shell->prev_pipe_in));
	if (!command->next)
		shell->last_pid = command->pid;
	if (command->next)
	{
		close(new_pipe[1]);
		(shell->prev_pipe_in) = new_pipe[0];
	}
	command = command->next;
	return (command);
}

t_command   *check_for_dots(t_command *command)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(command->args[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	command = command->next;
	return (command);
}

t_command	*check_for_exit(t_ms *shell, t_command *command, int *new_pipe)
{
	if (shell->child_count > 1)
		close(new_pipe[0]);
	ft_exit(command, shell);
	command = command->next;
	return (command);
}
