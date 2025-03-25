/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 20:09:42 by smishos           #+#    #+#             */
/*   Updated: 2025/03/19 20:09:43 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == ';');
}

void	check_for_append(t_ms *shell)
{
	realloc_buffer(shell);
	shell->buffer[1] = '>';
	shell->buffer[2] = '\0';
	shell->i++;
	shell->type = TOKEN_APPEND;
}

void	heredoc_case(t_ms *shell)
{
	realloc_buffer(shell);
	shell->buffer[1] = '<';
	shell->buffer[2] = '\0';
	shell->i++;
	shell->type = TOKEN_HERE_DOC;
}

void	if_is_operator(t_ms *shell)
{
	if (shell->buffer[0] == '|')
	{
		shell->type = TOKEN_PIPE;
		shell->pipe_count++;
	}
	else if (shell->buffer[0] == '<')
	{
		if (shell->input[shell->i + 1] == '<')
			heredoc_case(shell);
		else
		{
			shell->type = TOKEN_REDIR_IN;
			shell->rd_in_count++;
		}
	}
	else if (shell->buffer[0] == '>')
	{
		if (shell->input[shell->i + 1] == '>')
			check_for_append(shell);
		else
		{
			shell->type = TOKEN_REDIR_OUT;
			shell->rd_out_count++;
		}
	}
}

void	create_token(t_ms *shell)
{
	t_token	*new_token;

	if (!shell)
		return ;
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		malloc_error(shell);
	new_token->value = ft_strdup(shell->buffer);
	if (!new_token->value)
	{
		free(new_token);
		malloc_error(shell);
	}
	new_token->type = shell->type;
	new_token->next = NULL;
	add_token(shell, new_token);
}
