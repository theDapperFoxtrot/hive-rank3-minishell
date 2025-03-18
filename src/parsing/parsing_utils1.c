#include "../../include/minishell.h"

void	expand_env_var(t_ms *shell, int with_braces)
{
	shell->exp_i = 0;
	shell->exp_name_len = 0;
	shell->exp.value = qmark_check(shell, with_braces);
	get_var_name_len(shell, with_braces);
	shell->exp_temp_name = ft_substr(shell->exp.var_name, 0, shell->exp_name_len);
	free(shell->exp.var_name);
	shell->exp.var_name = NULL;
	if (!shell->exp_temp_name)
		return ;
	get_var_value(shell);
	free(shell->exp_temp_name);
}

int	handle_expansions_no_braces(t_ms *shell, const char *str)
{
	shell->exp.var_start = shell->exp.i + 1;
	shell->exp.var_len = 0;
	while (str[shell->exp.var_start + shell->exp.var_len] && \
	(ft_isalnum(str[shell->exp.var_start + shell->exp.var_len]) || \
	str[shell->exp.var_start + shell->exp.var_len] == '_' || \
	(shell->exp.var_len == 0 && str[shell->exp.var_start] == '?')))
		shell->exp.var_len++;
	if (shell->exp.var_len > 0)
	{
		shell->exp.var_name = ft_substr(str, shell->exp.var_start, shell->exp.var_len);
		expand_env_var(shell, 0);
		shell->exp.result = ft_realloc(shell->exp.result, shell->exp.j, shell->exp.j + ft_strlen(shell->exp.value) + 1);
		ft_strlcpy(shell->exp.result + shell->exp.j, shell->exp.value, ft_strlen(shell->exp.value) + 1);
		shell->exp.j += ft_strlen(shell->exp.value);
		shell->exp.i = shell->exp.var_start + shell->exp.var_len;
		shell->exp.value = free_and_nullify(shell->exp.value);
		return (1);
	}
	else
	{
		shell->exp.result = ft_realloc(shell->exp.result, shell->exp.j, shell->exp.j + 1);
		shell->exp.result[shell->exp.j++] = str[shell->exp.i++];
	}
	return (0);
}

int	handle_expansions_if_braces(t_ms *shell, const char *str)
{
	shell->exp.closing_brace = find_closing_brace(str, shell->exp.i + 1);
	if (shell->exp.closing_brace != -1)
	{
		shell->exp.var_name = ft_substr(str, shell->exp.i, shell->exp.closing_brace - shell->exp.i + 1);
		expand_env_var(shell, 1);
		shell->exp.result = ft_realloc(shell->exp.result, shell->exp.j, shell->exp.j + ft_strlen(shell->exp.value) + 1);
		ft_strlcpy(shell->exp.result + shell->exp.j, shell->exp.value, ft_strlen(shell->exp.value) + 1);
		shell->exp.j += ft_strlen(shell->exp.value);
		shell->exp.i = shell->exp.closing_brace + 1;
		return (1);
	}
	return (0);
}

int	handle_expansions_dollar_sign(t_ms *shell, const char *str)
{
	int		cont_loop;

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

void realloc_and_write(t_ms *shell, const char *str, int inc_i)
{
	if (inc_i)
		shell->exp.i++;
	shell->exp.result = ft_realloc(shell->exp.result, shell->exp.j, shell->exp.j + 1);
	if (!shell->exp.result)
		malloc_error(shell);
	shell->exp.result[shell->exp.j++] = str[shell->exp.i++];
}

int	go_through_exp_cases(t_ms *shell, const char *str)
{
	if (str[shell->exp.i] == '\'' || str[shell->exp.i] == '\"')
		shell->exp.result = handle_expansions_quotes(shell, str);
	else if (str[shell->exp.i] == '\\' && str[shell->exp.i + 1] == '$')
		realloc_and_write(shell, str, 1);
	else if (str[shell->exp.i] == '\\' && str[shell->exp.i + 1] == '\\')
		realloc_and_write(shell, str, 1);
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
		realloc_and_write(shell, str, 0);
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
	shell->exp.result = ft_realloc(shell->exp.result, shell->exp.j, shell->exp.j + 1);
	shell->exp.result[shell->exp.j] = '\0';
	return (shell->exp.result);
}
