/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 20:09:59 by smishos           #+#    #+#             */
/*   Updated: 2025/03/20 17:51:56 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*free_and_nullify(char *str_ptr)
{
	free(str_ptr);
	str_ptr = NULL;
	return (str_ptr);
}

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

char	*count_for_malloc(char *str)
{
	int		i;
	int		j;
	char	*parsed;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] != 34 && str[i] != 39)
			j++;
		i++;
	}
	parsed = malloc((j + 1) * sizeof(char));
	if (!parsed)
		return (NULL);
	return (parsed);
}

int	event(void)
{
	return (0);
}

void	ft_putstr_eq(char *str)
{
	int	i;

	if (!str)
		return ;
	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
		{
			ft_putchar_fd(str[i], 1);
			i++;
			if (str[i] != '\"')
				write(1, "\"", 1);
		}
		ft_putchar_fd(str[i], 1);
		i++;
	}
	if (str[i - 1] != '\"')
		write(1, "\"", 1);
}
