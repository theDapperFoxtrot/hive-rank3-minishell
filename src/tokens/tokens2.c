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
