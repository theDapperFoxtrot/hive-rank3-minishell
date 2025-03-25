/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 20:09:16 by smishos           #+#    #+#             */
/*   Updated: 2025/03/19 20:09:17 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	count_cmd_args(t_ms *shell, t_token *token)
{
	while (token && token->type != TOKEN_PIPE)
	{
		if (token->type == TOKEN_ARGS)
			shell->command_input_count++;
		else if (token->type == TOKEN_HERE_DOC)
			shell->command_input_count++;
		else if (token->type == TOKEN_REDIR_IN)
			shell->command_input_count++;
		else if (token->type == TOKEN_REDIR_OUT || token->type == TOKEN_APPEND)
			shell->command_input_count++;
		if (token->next)
		{
			if (token->next->type == TOKEN_PIPE)
			{
				token = token->next;
				token = token->next;
				shell->next_start = token;
				break ;
			}
			token = token->next;
		}
		else
			break ;
	}
}

void	check_for_append_lm(t_ms *shell)
{
	shell->buffer = ft_realloc(shell->buffer, shell->buf_count, \
					shell->buf_count + 1);
	if (!shell->buffer)
		malloc_error(shell);
	shell->buffer[1] = '>';
	shell->buffer[2] = '\0';
	shell->i++;
	shell->type = TOKEN_APPEND;
}

void	write_token_args_lm(t_ms *shell)
{
	shell->buf_i = 0;
	while (shell->pipe_rdl_tokens[shell->i] && \
	!isspace(shell->pipe_rdl_tokens[shell->i]) && \
	!is_operator(shell->pipe_rdl_tokens[shell->i]))
	{
		realloc_buffer(shell);
		if (shell->pipe_rdl_tokens[shell->i] == '"' || \
			shell->pipe_rdl_tokens[shell->i] == '\'')
			if_quotes(shell);
		else
			shell->buffer[shell->buf_i++] = shell->pipe_rdl_tokens[shell->i++];
	}
	shell->buffer[shell->buf_i] = '\0';
	if (shell->buf_i > 0)
		create_token(shell);
	shell->new_buffer = ft_realloc(shell->buffer, ft_strlen(shell->buffer), 1);
	if (!shell->new_buffer)
		malloc_error(shell);
	shell->new_buffer[0] = '\0';
	shell->buffer = shell->new_buffer;
}

void	checking_pipe_lm(t_ms *shell)
{
	shell->type = TOKEN_PIPE;
	shell->pipe_count++;
}

void	checking_rd_in_lm(t_ms *shell)
{
	if (shell->pipe_rdl_tokens[shell->i + 1] == '<')
	{
		shell->buffer[1] = '<';
		shell->buffer[2] = '\0';
		shell->i++;
		shell->type = TOKEN_HERE_DOC;
	}
	else
	{
		shell->type = TOKEN_REDIR_IN;
		shell->rd_in_count++;
	}
}
