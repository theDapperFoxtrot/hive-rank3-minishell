/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 20:09:49 by smishos           #+#    #+#             */
/*   Updated: 2025/03/19 20:09:50 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	read_file(t_ms *shell, char *file)
{
	int	fd_open;

	fd_open = open(file, O_RDONLY);
	if (fd_open == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(file);
		cleanup(shell, 1);
		exit(EXIT_FAILURE);
	}
	safe_dup2(fd_open, STDIN_FILENO);
	close(fd_open);
}

void	write_file(t_ms *shell, char *file)
{
	int	fd_write;

	fd_write = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_write == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(file);
		cleanup(shell, 1);
		exit(EXIT_FAILURE);
	}
	safe_dup2(fd_write, STDOUT_FILENO);
	close(fd_write);
}

void	append_file(t_ms *shell, char *file)
{
	int	fd_write;

	fd_write = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_write == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(file);
		cleanup(shell, 1);
		exit(EXIT_FAILURE);
	}
	safe_dup2(fd_write, STDOUT_FILENO);
	close(fd_write);
}
