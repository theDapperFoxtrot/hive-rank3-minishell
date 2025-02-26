#include "../../include/minishell.h"

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == ';');
}

void	check_for_append(t_ms *shell)
{
	shell->buffer[1] = '>';
	shell->buffer[2] = '\0';
	shell->i++;
	shell->type = TOKEN_APPEND;
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
		{
			realloc_buffer(shell);
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
	else if (shell->buffer[0] == '>')
	{
		if (shell->input[shell->i + 1] == '>')
		{
			check_for_append(shell);
			realloc_buffer(shell);
		}
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

	if (!shell || !shell->buffer)
		return ;
	new_token = malloc(sizeof(t_token));
	if (!new_token)
	{
		print_error("Error: malloc failed", shell, 1, 1);
		exit(shell->exit_code);
	}
	new_token->value = ft_strdup(shell->buffer);
	if (!new_token->value)
	{
		free(new_token);
		print_error("Error: malloc failed", shell, 1, 1);
		exit(shell->exit_code);
	}
	new_token->type = shell->type;
	new_token->next = NULL;
	add_token(shell, new_token);
}

void	realloc_buffer(t_ms *shell)
{
	shell->new_buffer = ft_realloc(shell->buffer, shell->buf_count, \
		shell->buf_count + 1);
	if (!shell->new_buffer)
	{
		print_error("Error: malloc failed", shell, 1, 1);
		exit(shell->exit_code);
	}
	shell->buffer = shell->new_buffer;
	shell->buf_count++;
}

void	if_quotes(t_ms *shell)
{
	char	quote;

	quote = shell->input[shell->i];
	realloc_buffer(shell);
	shell->buffer[shell->buf_i++] = shell->input[shell->i++];
	while (shell->input[shell->i] && shell->input[shell->i] != quote)
	{
		realloc_buffer(shell);
		shell->buffer[shell->buf_i++] = shell->input[shell->i++];
	}
	if (shell->input[shell->i] == quote)
	{
		realloc_buffer(shell);
		shell->buffer[shell->buf_i++] = shell->input[shell->i++];
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
	{
		print_error("Error: malloc failed", shell, 1, 1);
		exit(shell->exit_code);
	}
	shell->new_buffer[0] = '\0';
	shell->buffer = shell->new_buffer;
}
