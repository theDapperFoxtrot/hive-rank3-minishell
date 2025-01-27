/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 12:43:38 by saylital          #+#    #+#             */
/*   Updated: 2025/01/27 17:56:49 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Function to sort and print environment variables alphabetically
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
		printf("%s\n", copy_list[i]);
		i++;
	}
	free(copy_list);
}

void	set_env_variable(t_ms *shell, char *key, char *value)
{
	int		i;
	char	*temp;

	i = 0;
	while (shell->env_list[i])
	{
		if (ft_strncmp(shell->env_list[i], key, ft_strlen(key)) == 0)
		{
			free(shell->env_list[i]);
			temp = ft_strjoin(key, "=");
			shell->env_list[i] = ft_strjoin(temp, value);
			free(temp);
			return ;
		}
		i++;
	}
	shell->env_list = ft_realloc(shell->env_list, \
	sizeof(char *) * env_list_size(shell->env_list), \
	sizeof(char *) * (env_list_size(shell->env_list) + 2));
	temp = ft_strjoin(key, "=");
	shell->env_list[i] = ft_strjoin(temp, value);
	free(temp);
	i++;
	shell->env_list[i] = NULL;
}

void	export_error(char *arg)
{
	ft_putstr_fd("export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

int	export_command_check(char **command, t_ms *shell)
{
	if (!command[1])
	{
		print_sorted_env(shell);
		return (1);
	}
	return (0);
}

void	ft_export(char **command, t_ms *shell)
{
	char	*arg;
	char	*equal_sign;
	char	*key;
	char	*value;
	int		i;

	shell->commands->not_builtin = 0;
	if (export_command_check(command, shell))
		return ;
	i = 1;
	while (command[i])
	{
		arg = command[i];
		equal_sign = strchr(arg, '=');
		if (equal_sign)
		{
			*equal_sign = '\0';
			key = arg;
			value = equal_sign + 1;
			set_env_variable(shell, key, value);
		}
		else
			export_error(arg);
		i++;
	}
}

// single vs double quotes
// Hello '${USER}' = Hello ${USER}
// Hello "${USER}" = Hello Steve