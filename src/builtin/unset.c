#include "../../include/minishell.h"

void	unset_env_variable(t_ms *shell, char *key)
{
	int		i;
	int		j;
	char	**temp_list;
	int		list_size;

	list_size = env_list_size(shell->env_list);
	temp_list = (char **) malloc((list_size) * sizeof(char *));
	i = 0;
	j = 0;
	while (shell->env_list[i])
	{
		if (ft_strncmp(shell->env_list[i], key, ft_strlen(key)) == 0)
		{
			i++;
			continue ;
		}
		temp_list[j] = ft_strdup(shell->env_list[i]);
		i++;
		j++;
	}
	temp_list[j] = NULL;
	free_split(shell->env_list);
	shell->env_list = NULL;
	shell->env_list = temp_list;
}

int	does_var_exist(t_ms *shell, char *key)
{
	int	i;

	i = 0;
	while (shell->env_list[i])
	{
		if (ft_strncmp(shell->env_list[i], key, ft_strlen(key)) == 0)
			return (1);
		i++;
	}
	return (0);
}

void	ft_unset(char **command, t_ms *shell)
{
	int	i;

	shell->commands->not_builtin = 0;
	if (!command[1])
		return ;
	i = 1;
	while (command[i])
	{
		if (does_var_exist(shell, command[i]))
			unset_env_variable(shell, command[i]);
		i++;
	}
}
