/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:17:41 by saylital          #+#    #+#             */
/*   Updated: 2025/01/22 19:16:38 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	is_builtin(char **command, t_ms *shell)
{
	if (ft_strncmp(command[0], "echo", 4) == 0)
		ft_echo(command, shell);
	else if (ft_strncmp(command[0], "pwd", 3) == 0)
		ft_pwd(command, shell);
	else if (ft_strncmp(command[0], "exit", 4) == 0)
		ft_exit(command, shell);
	else if (ft_strncmp(command[0], "env", 3) == 0)
		ft_env(command, shell);
	else if (ft_strncmp(command[0], "cd", 2) == 0)
		ft_cd(command, shell);
	else if (ft_strncmp(command[0], "export", 6) == 0)
		ft_export(command, shell);
	else if (ft_strncmp(command[0], "unset", 5) == 0)
		ft_unset(command, shell);
}

// void	does_not_exist(char **command, t_ms *shell)
// {
// 	return ;
// }
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
	// t_command	*cmd;

	// cmd = shell->commands;
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

void	check_command(t_ms *shell)
{
	t_command	*command;

	command = shell->commands;
	while (command)
	{
		// if ((command->pid = fork()) == -1)
		// 	fork_error(shell);
		if (command->pid == 0)
		{
		if (command->redir_in && command->heredoc)
			read_file(shell->fd, command);
		if (command->redir_in)
			read_file(shell->fd, command);
		if (command->redir_out)
			write_file(shell->fd, command);
		if (command->append_mode)
			append_file(shell->fd, command);
		is_builtin(command->args, shell);
		// if (is_in_env(command, shell))
		// 	return ;
		// else
			// does_not_exist(command, shell)
		if (command->next)
			command = command->next;
		}
	}
	// close_fds(shell);
	// shell->exit_code = handle_exit(shell);
}

// void check_command(t_ms *shell) {
//     t_command *command;
//     command = shell->commands;
    
//     while (command) {
//         // Create new pipe for each command
//         if (pipe(shell->fd) == -1) {
//             ft_putendl_fd("minishell: pipe failed", 2);
//             exit(EXIT_FAILURE);
//         }
        
//         command->pid = fork();
//         if (command->pid == -1) {
//             ft_putendl_fd("minishell: fork failed", 2);
//             exit(EXIT_FAILURE);
//         }
        
//         if (command->pid == 0) {
//             // Child process
//             if (command->redir_in && command->heredoc) 
//                 read_file(shell->fd, command);
//             else if (command->redir_in)
//                 read_file(shell->fd, command);
//             if (command->redir_out)
//                 write_file(shell->fd, command);
//             if (command->append_mode)
//                 append_file(shell->fd, command);
                
//             is_builtin(command->args, shell);
//             exit(0); // Make sure child exits
//         } else {
//             // Parent process
//             close(shell->fd[0]);
//             close(shell->fd[1]);
//         }
        
//         command = command->next;
//     }
    
//     // Wait for all child processes
//     while (wait(NULL) > 0);
// }
// void read_file(int *fd, t_command *cmd) {
//     int fd_open;
    
//     fd_open = open(cmd->args[1], O_RDONLY);
//     if (fd_open == -1) {
//         ft_putstr_fd("minishell: ", 2);
//         perror(cmd->args[1]);
//         exit(1);
//     }
    
//     safe_dup2(fd_open, STDIN_FILENO);
//     if (cmd->next) // Only redirect to pipe if there's a next command
//         safe_dup2(fd[1], STDOUT_FILENO);
    
//     close(fd[0]); // Close read end
//     if (fd_open != STDIN_FILENO)
//         close(fd_open);
// }

