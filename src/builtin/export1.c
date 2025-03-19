#include "../../include/minishell.h"

void	print_sorted_env(t_ms *shell)
{
	int		i;
	int		size;
	char	**copy_list;

	size = env_list_size(shell->env_list);
	copy_list = malloc((size + 1) * sizeof(char *));
	if (!copy_list)
		return ;
	i = 0;
	while (i < size)
	{
		copy_list[i] = shell->env_list[i];
		i++;
	}
	copy_list[size] = NULL;
	quicksort(copy_list, 0, size - 1);
	i = 0;
	while (copy_list[i])
	{
		ft_putstr_fd(copy_list[i], 1);
		ft_putstr_fd("\n", 1);
		i++;
	}
	free(copy_list);
}

void	export_error(t_ms *shell, char *arg)
{
	ft_putstr_fd("export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	shell->exit_code = 1;
}

void	if_value_not_empty(t_ms *shell, char *key, char *value, int i)
{
	char	*temp;

	temp = ft_strjoin(key, "=\"");
	shell->env_list[i] = ft_strjoin(temp, value);
	free(temp);
	temp = shell->env_list[i];
	shell->env_list[i] = ft_strjoin(temp, "\"");
	free(temp);
}

void	if_equal_found(t_ms *shell, char *key, int i)
{
	char	*temp;

	temp = ft_strjoin(key, "=\"\"");
	shell->env_list[i] = ft_strdup(temp);
	free(temp);
}

int	sift_through_env(t_ms *shell, char *key, char *value, int i)
{
	if (ft_strncmp(shell->env_list[i], key, ft_strlen(key)) == 0)
	{
		free(shell->env_list[i]);
		if (ft_strlen(value) > 0)
			if_value_not_empty(shell, key, value, i);
		else if (shell->equal_found)
			if_equal_found(shell, key, i);
		else if (ft_strlen(value) == 0)
			shell->env_list[i] = ft_strdup(key);
		return (1);
	}
	return (0);
}
