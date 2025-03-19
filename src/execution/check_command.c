#include "../../include/minishell.h"

t_command	*checking_for_select_commands(t_ms *shell, \
			t_command *command, int *new_pipe)
{
	if ((ft_strncmp(command->args[0], "..", 2) == 0 && \
		ft_strlen(command->args[0]) == 2))
	{
		command = check_for_dots(command);
		shell->select_command_found = 1;
		return (command);
	}
	if (ft_strncmp(command->args[0], "exit", 4) == 0 && \
		ft_strlen(command->args[0]) == 4)
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

void	pipe_failure(t_ms *shell)
{
	ft_putendl_fd("minishell: pipe failed", 2);
	cleanup(shell, 1);
	exit(EXIT_FAILURE);
}

void	starting_values(t_ms *shell)
{
	shell->prev_pipe_in = -1;
	shell->last_pid = 0;
	shell->child_count = 0;
	shell->select_command_found = 0;
}

pid_t	call_fork(t_ms *shell, t_command *command, int *new_pipe)
{
	shell->child_count++;
	command->pid = fork();
	if (command->pid == -1)
		fork_error(new_pipe);
	return (command->pid);
}

void	check_command(t_ms *shell, t_command *command)
{
	int			new_pipe[2];

	starting_values(shell);
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
		command->pid = call_fork(shell, command, new_pipe);
		if (command->pid == 0)
			child_process(shell, command, new_pipe);
		else
			command = parent_process(shell, command, new_pipe);
	}
	if (shell->prev_pipe_in != -1)
		close(shell->prev_pipe_in);
	wait_for_kids(shell);
}
