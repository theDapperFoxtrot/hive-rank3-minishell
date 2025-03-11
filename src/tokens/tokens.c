#include "../../include/minishell.h"

//if very first token, set it to new token
//otherwise, iterate to the end of the list, and add next new token
void	add_token(t_ms *shell, t_token *new_token)
{
	t_token	*current;

	current = shell->token;
	if (!new_token || !shell)
		return ;
	if (!shell->token)
	{
		shell->token = new_token;
		new_token->next = NULL;
		return ;
	}
	while (current->next)
		current = current->next;
	current->next = new_token;
	new_token->next = NULL;
}

int	lead_pipe_check(t_ms *shell, int lead_pipe)
{
	if (shell->input[shell->i] == '|' && lead_pipe)
	{
		if (shell->input[shell->i] == '|' && shell->input[shell->i + 1] == '|')
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `||'\n" \
				, 2);
				shell->exit_code = 2;
			free_tokens(shell);
			return (1);
		}
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		shell->exit_code = 2;
		free_tokens(shell);
		return (1);
	}
	return (0);
}

int	is_operator_true(t_ms *shell)
{
	if (is_operator(shell->input[shell->i]))
	{
		realloc_buffer(shell);
		shell->buffer[0] = shell->input[shell->i];
		shell->buffer[1] = '\0';
		if_is_operator(shell);
		create_token(shell);
		shell->type = TOKEN_ARGS;
		shell->i++;
		shell->new_buffer = ft_realloc(shell->buffer, \
			ft_strlen(shell->buffer), 1);
		if (!shell->new_buffer)
			malloc_error(shell);
		shell->new_buffer[0] = '\0';
		shell->buffer = shell->new_buffer;
		shell->buf_count = 1;
		return (1);
	}
	return (0);
}

void	tokenize_input(t_ms *shell)
{
	int		lead_pipe;
	lead_pipe = 1;
	shell->i = 0;
	shell->buf_count = 1;
	shell->token = NULL;
	while (shell->input[shell->i])
	{
		while (shell->input[shell->i] && ft_isspace(shell->input[shell->i]))
			shell->i++;
		if (lead_pipe_check(shell, lead_pipe))
			return ;
		if (!shell->input[shell->i])
			break ;
		lead_pipe = 0;
		if (is_operator_true(shell))
			continue ;
		write_token_args(shell);
		shell->buf_count = 1;
	}
	if (shell->buffer)
	{
		free(shell->buffer);
		shell->buffer = NULL;
	}
}

// Function to check if character is a special shell character
void	write_token_args(t_ms *shell)
{
	shell->buf_i = 0;
	while (shell->input[shell->i] && \
	!ft_isspace(shell->input[shell->i]) && \
	!is_operator(shell->input[shell->i]))
	{
		realloc_buffer(shell);
		if (shell->input[shell->i] == '"' || shell->input[shell->i] == '\'')
			if_quotes(shell);
		else
			shell->buffer[shell->buf_i++] = shell->input[shell->i++];
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
