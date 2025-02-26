#include "../../include/minishell.h"

void	add_token(t_ms *shell, t_token *new_token)
{
	t_token	*current;

	current = shell->token;
	if (!new_token || !shell)
		return ;
	if (!shell->token) //if very first token, set it to new token
	{
		shell->token = new_token;
		new_token->next = NULL;
		return ;
	}
	while (current->next) //otherwise, iterate to the end of the list, and add next new token
		current = current->next;
	current->next = new_token;
	new_token->next = NULL;
}

int lead_pipe_check(t_ms *shell, int lead_pipe)
{
	if (shell->input[shell->i] == '|' && lead_pipe)
	{
		if (shell->input[shell->i] == '|' && shell->input[shell->i + 1] == '|')
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `||'\n", 2);
			free_tokens(shell);
			return (1);
		}
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		free_tokens(shell);
		return (1);
	}
	return (0);
}

int is_operator_true(t_ms *shell)
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
		shell->new_buffer = ft_realloc(shell->buffer, ft_strlen(shell->buffer), 1);
		if (!shell->new_buffer)
		{
			print_error("Error: malloc failed", shell, 1, 1);
			exit(shell->exit_code);
		}
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
	shell->token = NULL;
	shell->i = 0;
	shell->buf_count = 1;
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
