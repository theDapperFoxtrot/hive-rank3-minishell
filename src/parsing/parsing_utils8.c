/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils8.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 20:09:24 by smishos           #+#    #+#             */
/*   Updated: 2025/03/24 17:33:04 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	expand_env_var(t_ms *shell, int with_braces)
{
	shell->exp_i = 0;
	shell->exp_name_len = 0;
	shell->exp.value = qmark_check(shell, with_braces);
	get_var_name_len(shell, with_braces);
	shell->exp_temp_name = ft_substr(shell->exp.var_name, \
		0, shell->exp_name_len);
	free(shell->exp.var_name);
	shell->exp.var_name = NULL;
	if (!shell->exp_temp_name)
		return ;
	get_var_value(shell);
	free(shell->exp_temp_name);
}

void	var_len_not_zero(t_ms *shell, const char *str)
{
	shell->exp.var_name = ft_substr(str, shell->exp.var_start, \
									shell->exp.var_len);
	expand_env_var(shell, 0);
	shell->exp.result = ft_realloc(shell->exp.result, shell->exp.j, \
		shell->exp.j + ft_strlen(shell->exp.value) + 1);
	ft_strlcpy(shell->exp.result + shell->exp.j, \
		shell->exp.value, ft_strlen(shell->exp.value) + 1);
	shell->exp.j += ft_strlen(shell->exp.value);
	shell->exp.i = shell->exp.var_start + shell->exp.var_len;
	shell->exp.value = free_and_nullify(shell->exp.value);
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
		var_len_not_zero(shell, str);
		return (1);
	}
	else
	{
		shell->exp.result = ft_realloc(shell->exp.result, \
			shell->exp.j, shell->exp.j + 1);
		shell->exp.result[shell->exp.j++] = str[shell->exp.i++];
	}
	return (0);
}

void	null_hd_and_oneline(t_ms *shell, t_command *cmd, int i)
{
	shell->heredoc_line_count = shell->heredoc_line_count + i;
	cmd->heredoc_lines[i] = NULL;
	make_heredoc_one_line(shell, cmd);
	cmd->heredoc = 1;
	cmd->command_input_index++;
}

int	print_ret(t_ms *shell, char *message)
{
	print_error(message, shell, 2, 0);
	shell->token_error = 1;
	return (1);
}
