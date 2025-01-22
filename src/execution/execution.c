#include "../../include/minishell.h"

char	*get_paths(char **cmd, t_ms *shell)
{
	static char	*path;
	int			i;

	i = 0;
	while (shell->env_list[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path = ft_strdup(envp[i] + 5);
			break ;
		}
		i++;
	}
	// if (!path && access(cmd, X_OK))
		// error_exit(pipex, cmd, "No such file or directory\n", 127);
	return (path);
}

int	ft_slash_check(char **cmd, t_ms *shell)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '/')
		{
			if (!access(cmd, F_OK))
			{
				if (!access(cmd, X_OK))
					return (1);
				else
					perm_denied_and_exit(pipex, cmd);
			}
			else
				no_such_file_and_exit(pipex, cmd);
		}
		i++;
	}
	return (0);
}

void	check_env(char **command, t_ms *shell)
{
	char	*path;

	if (ft_slash_check(pipex, pipex->cmd_tokens[0]))
	{
		if (pipex->i == 0)
			malloc_abs_path1(pipex, pipex->cmd_tokens);
		else
			malloc_abs_path2(pipex, pipex->cmd_tokens);
		free_split(pipex->cmd_tokens);
		return ;
	}
	path = get_paths(pipex, envp, command);
	if (loop_paths(pipex, path, pipex->cmd_tokens))
		free_path_and_tokens(pipex, path);
	else
	{
		free_path_and_tokens(pipex, path);
		cmd_not_found_exit(pipex, command);
	}
}
