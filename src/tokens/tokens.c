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
		// new_token->next = NULL;
		return ;
	}
	while (current->next) //otherwise, iterate to the end of the list, and add next new token
		current = current->next;
	current->next = new_token;
	// new_token->next = NULL;
}

void	tokenize_input(t_ms *shell)
{
	int		lead_pipe;

	lead_pipe = 1;
	shell->token = NULL;
	shell->i = 0;
	while (shell->input[shell->i])
	{
		while (shell->input[shell->i] && ft_isspace(shell->input[shell->i]))
			shell->i++;
		if (shell->input[shell->i] == '|' && lead_pipe)
		{
			print_error("minishell: syntax error near unexpected token `|'", shell, 2, 0);
			return ;
		}
		if (!shell->input[shell->i])
			break ;
		lead_pipe = 0;
		if (is_operator(shell->input[shell->i]))
		{
			shell->buffer[0] = shell->input[shell->i];
			shell->buffer[1] = '\0';
			if_is_operator(shell);
			create_token(shell);
			shell->type = TOKEN_ARGS;
			shell->i++;
			continue ;
		}
		write_token_args(shell);
	}
}
