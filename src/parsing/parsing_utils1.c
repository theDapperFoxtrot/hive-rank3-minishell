/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 20:09:05 by smishos           #+#    #+#             */
/*   Updated: 2025/03/19 20:09:06 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_expansions_if_braces(t_ms *shell, const char *str)
{
	shell->exp.closing_brace = find_closing_brace(str, shell->exp.i + 1);
	if (shell->exp.closing_brace != -1)
	{
		shell->exp.var_name = ft_substr(str, shell->exp.i, \
			shell->exp.closing_brace - shell->exp.i + 1);
		expand_env_var(shell, 1);
		shell->exp.result = ft_realloc(shell->exp.result, shell->exp.j, \
			shell->exp.j + ft_strlen(shell->exp.value) + 1);
		ft_strlcpy(shell->exp.result + shell->exp.j, shell->exp.value, \
			ft_strlen(shell->exp.value) + 1);
		shell->exp.j += ft_strlen(shell->exp.value);
		shell->exp.i = shell->exp.closing_brace + 1;
		return (1);
	}
	return (0);
}

int	handle_expansions_dollar_sign(t_ms *shell, const char *str)
{
	int	cont_loop;

	cont_loop = 0;
	if (str[shell->exp.i + 1] == '{')
	{
		if (str[shell->exp.i + 2] == '\'' || str[shell->exp.i + 2] == '\"')
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putconstr_fd(str, 2);
			ft_putstr_fd(": bad substitution\n", 2);
			return (-1);
		}
		cont_loop = handle_expansions_if_braces(shell, str);
	}
	else
		cont_loop = handle_expansions_no_braces(shell, str);
	return (cont_loop);
}

void	realloc_and_write(t_ms *shell, const char *str, int inc_i, int write)
{
	if (inc_i)
		shell->exp.i++;
	shell->exp.result = ft_realloc(shell->exp.result, \
		shell->exp.j, shell->exp.j + 1);
	if (!shell->exp.result)
		malloc_error(shell);
	if (write)
		shell->exp.result[shell->exp.j++] = str[shell->exp.i++];
}

int	go_through_exp_cases(t_ms *shell, const char *str)
{
	if (str[shell->exp.i] == '\'' || str[shell->exp.i] == '\"')
		shell->exp.result = handle_expansions_quotes(shell, str);
	else if (str[shell->exp.i] == '\\' && str[shell->exp.i + 1] == '$')
		realloc_and_write(shell, str, 1, 1);
	else if (str[shell->exp.i] == '\\' && str[shell->exp.i + 1] == '\\')
		realloc_and_write(shell, str, 1, 1);
	else if (str[shell->exp.i] == '$')
	{
		if (str[shell->exp.i + 1] == '\'' || str[shell->exp.i + 1] == '\"')
		{
			shell->exp.i++;
			shell->exp.result = handle_expansions_quotes(shell, str);
		}
		if (handle_expansions_dollar_sign(shell, str) == -1)
			return (0);
		else
			return (1);
	}
	else
		realloc_and_write(shell, str, 0, 1);
	return (1);
}

char	*handle_expansions(t_ms *shell, const char *str)
{
	shell->exp.i = 0;
	shell->exp.j = 0;
	shell->exp.result = ft_strdup("");
	if (!(shell->exp.result))
		malloc_error(shell);
	while (str[shell->exp.i])
	{
		if (!go_through_exp_cases(shell, str))
			return (NULL);
		continue ;
	}
	shell->exp.value = free_and_nullify(shell->exp.value);
	shell->exp.var_name = free_and_nullify(shell->exp.var_name);
	shell->exp.result = ft_realloc(shell->exp.result, shell->exp.j, \
		shell->exp.j + 1);
	shell->exp.result[shell->exp.j] = '\0';
	return (shell->exp.result);
}
