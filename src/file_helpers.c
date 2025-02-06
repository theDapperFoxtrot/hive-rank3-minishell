#include "../include/minishell.h"

void	safe_dup2(int fd, int fd2)
{
	if (dup2(fd, fd2) == -1)
	{
		ft_putendl_fd("minishell: dup2 failed", 2);
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
}

void	read_file(t_command *cmd, int i)
{
	int	fd_open;

	fd_open = open(cmd->input_file[i], O_RDONLY);
	if (fd_open == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(cmd->input_file[i]);
		free(cmd->input_file[i]);
		exit(EXIT_FAILURE);
	}
	// if (access(cmd->input_file[i], F_OK))
	// 	ft_putstr_fd("No such file or directory", 2);
	// else if (access(cmd->input_file[i], R_OK))
	// 	ft_putstr_fd("Permission denied", 2);
	if (i != cmd->free_input_count - 1)
	{
		close(fd_open);
		return ;
	}
	safe_dup2(fd_open, STDIN_FILENO);
	close(fd_open);
}

void	read_heredoc(t_command *cmd)
{
		write(STDOUT_FILENO, cmd->heredoc_line, ft_strlen(cmd->heredoc_line));
		free(cmd->heredoc_line);
}

void	write_file(t_command *cmd, int i)
{
	int	fd_write;

	fd_write = open(cmd->output_file[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_write == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(cmd->output_file[i]);
		exit(EXIT_FAILURE);
	}
	// if (access(cmd->output_file[i], F_OK))
	// 	ft_putstr_fd("No such file or directory", 2);
	// else if (access(cmd->output_file[i], W_OK))
	// 	ft_putstr_fd("Permission denied", 2);
	if (i != cmd->free_output_count - 1)
	{
		close(fd_write);
		return ;
	}
	safe_dup2(fd_write, STDOUT_FILENO);
	close(fd_write);
}

void	append_file(t_command *cmd, int i)
{
	int	fd_write;

	fd_write = open(cmd->output_file[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_write == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(cmd->output_file[i]);
		exit(EXIT_FAILURE);
	}
	safe_dup2(fd_write, STDOUT_FILENO);
	close(fd_write);
}