/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 20:09:26 by smishos           #+#    #+#             */
/*   Updated: 2025/03/24 17:20:32 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*handle_token_redir(t_ms *shell, t_command *cmd, t_token *token, \
							void (*func)(t_ms *, t_command *, t_token *))
{
	if (pipe_syntax_check(shell, token))
		return (NULL);
	if (token && token->next && (token->next->type == TOKEN_REDIR_IN || \
		token->next->type == TOKEN_REDIR_OUT))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putstr_fd(token->next->value, 2);
		ft_putstr_fd("'\n", 2);
		shell->exit_code = 2;
		shell->token_error = 1;
		return (NULL);
	}
	func(shell, cmd, token);
	return (token);
}

t_token	*check_token_redir(t_ms *shell, t_command *cmd, t_token *token)
{
	if ((token->type == TOKEN_REDIR_IN) && token->next)
	{
		if (!handle_token_redir(shell, cmd, token, &handle_token_redir_in))
			return (NULL);
	}
	else if (token->type == TOKEN_HERE_DOC && token->next)
	{
		if (handle_token_heredoc(shell, cmd, token) == 1)
			return (NULL);
		if (g_signal == SIGINT)
			return (NULL);
	}
	else if ((token->type == TOKEN_REDIR_OUT) && token->next)
	{
		if (!handle_token_redir(shell, cmd, token, &handle_token_redir_out))
			return (NULL);
	}
	else if (token->type == TOKEN_APPEND && token->next)
	{
		if (!handle_token_redir(shell, cmd, token, &handle_token_append))
			return (NULL);
	}
	if (token->next != NULL)
		token = token->next;
	return (token);
}

t_command	*setup_token(t_ms *shell, t_command *cmd, t_token *token)
{
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	ft_bzero(cmd, sizeof(t_command));
	shell->commands = cmd;
	shell->command_input_count = 0;
	shell->command_input_count = setup_command_input_count(shell, cmd, token);
	cmd->command_input_index = 0;
	cmd->next = NULL;
	return (cmd);
}

void	handle_not_next_token(t_ms *shell, t_command *cmd, t_token *token)
{
	if (cmd->command_input)
		cmd->command_input[cmd->command_input_index] = NULL;
	if ((ft_strncmp(token->value, ">", 1) == 0) || \
		(ft_strncmp(token->value, "<", 1) == 0))
	{
		print_error("minishell: syntax error near unexpected token `newline'", \
			shell, 2, 0);
		shell->token_error = 1;
		return ;
	}
}

void	parse_tokens(t_ms *shell)
{
	t_token		*token;
	t_command	*cmd;

	token = shell->token;
	cmd = NULL;
	cmd = setup_token(shell, cmd, token);
	while (token)
	{
		if (token->type == TOKEN_ARGS)
			handle_token_args(shell, cmd, token);
		else if (token->type == TOKEN_PIPE)
			cmd = new_command(shell, cmd, token);
		else
		{
			token = check_token_redir(shell, cmd, token);
			if (!token)
				return ;
		}
		if (token->next == NULL)
		{
			handle_not_next_token(shell, cmd, token);
			break ;
		}
		token = token->next;
	}
}
