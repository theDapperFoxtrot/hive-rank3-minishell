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

void	read_file(int *fd, t_command *cmd)
{
	int	fd_open;

	close(fd[0]);
	fd_open = open(cmd->args[1], O_RDONLY);
	if (fd_open == -1)
	{
		close(fd[1]);
		ft_putstr_fd("minishell: ", 2);
		perror(cmd->args[1]);
		exit(0);
	}
	safe_dup2(fd_open, STDIN_FILENO);
	safe_dup2(fd[1], STDOUT_FILENO);
}

void	write_file(int *fd, t_command *cmd)
{
	int	fd_write;

	close(fd[1]);
	fd_write = open(cmd->args[cmd->arg_count - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_write == -1)
	{
		close(fd[0]);
		ft_putstr_fd("minishell: ", 2);
		perror(cmd->args[cmd->arg_count - 1]);
		exit(1);
	}
	safe_dup2(fd_write, STDOUT_FILENO);
	safe_dup2(fd[0], STDIN_FILENO);
}

void	append_file(int *fd, t_command *cmd)
{
	int	fd_write;

	close(fd[1]);
	fd_write = open(cmd->args[cmd->arg_count - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_write == -1)
	{
		close(fd[0]);
		ft_putstr_fd("minishell: ", 2);
		perror(cmd->args[cmd->arg_count - 1]);
		exit(1);
	}
	safe_dup2(fd_write, STDOUT_FILENO);
	safe_dup2(fd[0], STDIN_FILENO);
}