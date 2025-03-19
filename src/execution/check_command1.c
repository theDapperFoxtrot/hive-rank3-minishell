#include "../../include/minishell.h"

int	is_parent_builtin(char **command, t_ms *shell)
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
	else if (ft_strncmp(command[0], "pwd", 3) == 0 && \
		ft_strlen(command[0]) == 3)
	{
		ft_pwd(shell);
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
	else if ((ft_strncmp(command[0], "echo", 4) == 0 && \
		ft_strlen(command[0]) == 4) || \
		(ft_strncmp(command[0], "/bin/echo", 9) == 0 && \
		ft_strlen(command[0]) == 9))
	{
		ft_echo(command, shell);
		return (1);
	}
	else if (ft_strncmp(command[0], "env", 3) == 0 && \
			ft_strlen(command[0]) == 3)
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
