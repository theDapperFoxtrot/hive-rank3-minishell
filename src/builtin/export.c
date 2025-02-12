/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 12:43:38 by saylital          #+#    #+#             */
/*   Updated: 2025/02/12 17:57:24 by smishos          ###   ########.fr       */
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

void	export_error(t_ms *shell, char *arg)
{
	ft_putstr_fd("export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	shell->exit_code = 1;
}
void	set_env_variable(t_ms *shell, char *key, char *value, char *equal_sign)
{
	int		i;
	char	*temp;

	i = 0;
	if (!ft_isalpha(key[i]) && key[i] != '_')
	{
		export_error(shell, key);
		shell->exit_code = 1;
		return ;
	}
	i++;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
		{
			export_error(shell, key);
			shell->exit_code = 1;
			return ;
		}
		i++;
	}
	while (shell->env_list[i])
	{
		if (ft_strncmp(shell->env_list[i], key, ft_strlen(key)) == 0)
		{
			free(shell->env_list[i]);
			if (ft_strlen(value) > 0)
			{
				temp = ft_strjoin(key, "=");
				shell->env_list[i] = ft_strjoin(temp, value);
				free(temp);
			}
			else if (shell->equal_found)
			{
				temp = ft_strjoin(key, "=\"\"");
				shell->env_list[i] = ft_strdup(temp);
				free(temp);
			}
			else if (ft_strlen(value) == 0)
				shell->env_list[i] = ft_strdup(key);
			return ;
		}
		i++;
	}
	if (shell->equal_found)
		equal_sign[0] = '=';
	shell->env_list = ft_realloc(shell->env_list, \
	sizeof(char *) * env_list_size(shell->env_list), \
	sizeof(char *) * (env_list_size(shell->env_list) + 2));
	if (ft_strchr(key, '=') && !value)
	{
		temp = ft_strjoin(key, "\"\"");
		shell->env_list[i] = ft_strdup(temp);
		free(temp);
	}
	else
		shell->env_list[i] = ft_strdup(key);
	i++;
	shell->env_list[i] = NULL;
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

	
	if (export_command_check(command, shell))
		return ;
	i = 1;
	shell->equal_found = 0;
	while (command[i])
	{
		arg = command[i];
		equal_sign = ft_strchr(arg, '=');
		if (arg[0] == '=')
		{
			export_error(shell, arg);
			i++;
			continue ;
		}
		if (!equal_sign)
		{
			shell->equal_found = 0;
			key = arg;
			value = '\0';
			set_env_variable(shell, key, value, equal_sign);
		}
		else if (equal_sign)
		{
			shell->equal_found = 1;
			*equal_sign = '\0';
			key = arg;
			value = equal_sign + 1;
			set_env_variable(shell, key, value, equal_sign);
		}
		else
		{
			export_error(shell, arg);
		}
		i++;
	}
}

// single vs double quotes
// Hello '${USER}' = Hello ${USER}
// Hello "${USER}" = Hello Steve