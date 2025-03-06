/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:33:02 by saylital          #+#    #+#             */
/*   Updated: 2025/03/06 14:11:14 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	validate_str(char *str, char *acc_values)
{
	size_t	s_i;
	size_t	a_i;

	s_i = 0;
	if (str == NULL || acc_values == NULL)
		return (-1);
	while (str[s_i] != '\0')
	{
		a_i = 0;
		while (acc_values[a_i] != '\0')
		{
			if (str[s_i] == acc_values[a_i])
				break ;
			a_i++;
			if (acc_values[a_i] == '\0')
				return (-1);
		}
		s_i++;
	}
	return (1);
}

int	is_overflown(char *str)
{
	long long	nbr;
	size_t		len;

	if (ft_strncmp("-9223372036854775808", str, 20) == 0)
		return (1);
	nbr = ft_atol(str);
	len = ft_strlen(str);
	if ((nbr == 0 && len != 1) || (nbr == -1 && len != 2))
		return (-1);
	return (1);
}

long long	exit_error_check(t_ms *shell, char **cmd)
{
	long long	ret_val;

	ret_val = LLONG_MIN;
	if (cmd[1] != NULL)
	{
		if (validate_str(cmd[1], "-+0123456789") == -1 \
			|| is_overflown(cmd[1]) < 0)
		{
			ft_putstr_fd("exit\nminishell: exit: ", 2);
			ft_putstr_fd(cmd[1], 2);
			print_error(": numeric argument required", shell, 2, 1);
			shell->exit_code = 2;
			exit(shell->exit_code);
		}
		else
			ret_val = ft_atol(cmd[1]);
	}
	return (ret_val);
}


static void	check_numeric(t_command *command, t_ms *shell)
{
	int	i;
	long long ret_value;

	i = 0;
	if (!command->args[1])
		return ;
	if (command->args[1][i] == '#')
	{
		shell->exit_code = 0;
		ft_putstr_fd("exit\n", 2);
		cleanup(shell, 1);
		exit(shell->exit_code);
	}
	while (command->args[1][i])
	{
		if (ft_isalpha(command->args[1][i]) == 1)
		{
			shell->exit_code = 2;
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(command->args[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			if (ft_strncmp(command->args[1], "false", 5) == 0)
				shell->exit_code = 1;
			if (ft_strncmp(command->args[1], "true", 4) == 0)
				shell->exit_code = 0;
			cleanup(shell, 1);
			exit(shell->exit_code);
		}
		i++;
	}
	ret_value = exit_error_check(shell, command->args);
	if (ret_value && shell->exit_error_flag == 1)
	{
		print_error("exit: too many arguments", shell, 2, 1);
		exit(shell->exit_code);
	}
}

void	ft_exit(t_command *command, t_ms *shell)
{
	if (!command->args[1] && !command->next)
	{
		ft_putstr_fd("exit\n", 2);
		cleanup(shell, 1);
		exit(shell->exit_code);
	}
	check_numeric(command, shell);
	if (command->arg_count > 2)
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		shell->exit_code = 1;
		return ;
	}
	else if (command->args[1] && !command->next)
	{
		shell->exit_code = ft_atoi(command->args[1]);
		ft_putstr_fd("exit\n", 1);
		cleanup(shell, 1);
		exit(shell->exit_code);
	}
}
