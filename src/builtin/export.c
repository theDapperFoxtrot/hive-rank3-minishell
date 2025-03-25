/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saylital <saylital@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 12:43:38 by saylital          #+#    #+#             */
/*   Updated: 2025/03/19 17:39:05 by saylital         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	export_command_check(char **command, t_ms *shell)
{
	if (!command[1])
	{
		print_sorted_env(shell);
		return (1);
	}
	return (0);
}

void	found_equal_sign(t_ms *shell, char *arg, char *key, char *equal_sign)
{
	char	*value;

	shell->equal_found = 1;
	*equal_sign = '\0';
	key = arg;
	value = equal_sign + 1;
	set_env_variable(shell, key, value, equal_sign);
}

void	no_equal_sign(t_ms *shell, char *arg, char *key, char *equal_sign)
{
	char	*value;

	shell->equal_found = 0;
	key = arg;
	value = 0;
	set_env_variable(shell, key, value, equal_sign);
}

int	first_char_is_equal(t_ms *shell, char *arg, int *i)
{
	if (arg[0] == '=')
	{
		export_error(shell, arg);
		(*i)++;
		return (1);
	}
	return (0);
}

void	ft_export(char **command, t_ms *shell)
{
	char	*arg;
	char	*equal_sign;
	char	*key;
	int		i;

	key = NULL;
	if (export_command_check(command, shell))
		return ;
	i = 1;
	shell->equal_found = 0;
	while (command[i])
	{
		arg = command[i];
		equal_sign = ft_strchr(arg, '=');
		if (first_char_is_equal(shell, arg, &i))
			continue ;
		if (!equal_sign)
			no_equal_sign(shell, arg, key, equal_sign);
		else if (equal_sign)
			found_equal_sign(shell, arg, key, equal_sign);
		else
			export_error(shell, arg);
		i++;
	}
}
