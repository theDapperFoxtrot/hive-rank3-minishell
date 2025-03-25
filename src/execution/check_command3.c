/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_command3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 20:08:51 by smishos           #+#    #+#             */
/*   Updated: 2025/03/20 14:55:35 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	hir_hd_child_process(t_ms *shell, t_command *command, int *pipefd)
{
	close(pipefd[0]);
	if (write(pipefd[1], command->heredoc_line, \
		ft_strlen(command->heredoc_line)) == -1)
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

void	handle_input_redirection(t_ms *shell, t_command *command, \
								char *symbol, char *file)
{
	int	pipefd[2];

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
			parent_wait(shell, command, pipefd);
	}
	else if (ft_strncmp(symbol, "<", 1) == 0)
		read_file(shell, file);
}

void	handle_output_redirection(t_ms *shell, char *symbol, char *file)
{
	if (ft_strncmp(symbol, ">>", 2) == 0)
	{
		if (shell->child_count > 0)
			write_file(shell, file);
		else
			append_file(shell, file);
	}
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
		handle_output_redirection(shell, command->command_input[i], \
								command->command_input[i + 1]);
	if (ft_strncmp(command->command_input[i], "<<", 2) == 0)
		handle_input_redirection(shell, command, command->command_input[i], \
								command->command_input[i + 1]);
	if (ft_strncmp(command->command_input[i], ">", 1) == 0)
		handle_output_redirection(shell, command->command_input[i], \
								command->command_input[i + 1]);
	if (ft_strncmp(command->command_input[i], "<", 1) == 0)
		handle_input_redirection(shell, command, command->command_input[i], \
								command->command_input[i + 1]);
}
