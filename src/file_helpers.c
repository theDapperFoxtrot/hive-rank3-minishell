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

	fd_open = open(cmd->input_file, O_RDONLY);
	if (fd_open == -1)
	{
		close(fd[1]);
		ft_putstr_fd("minishell: readfile error\n", 2);
		perror(cmd->input_file);
		exit(0);
	}
	safe_dup2(fd_open, STDIN_FILENO);
	close(fd_open);
}

void	write_file(int *fd, t_command *cmd)
{
	int	fd_write;

	fd_write = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_write == -1)
	{
		close(fd[0]);
		ft_putstr_fd("minishell: writefile error\n", 2);
		perror(cmd->output_file);
		exit(1);
	}
	safe_dup2(fd_write, STDOUT_FILENO);
	close(fd_write);
}

void	append_file(int *fd, t_command *cmd)
{
	int	fd_write;

	close(fd[1]);
	fd_write = open(cmd->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_write == -1)
	{
		close(fd[0]);
		ft_putstr_fd("minishell: append file error\n", 2);
		perror(cmd->output_file);
		exit(1);
	}
	safe_dup2(fd_write, STDOUT_FILENO);
	safe_dup2(fd[0], STDIN_FILENO);
}