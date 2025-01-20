#include "../../include/minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

void	tokenize_input(t_ms *shell)
{
	shell->token = NULL;
	shell->i = 0;
	while (shell->input[shell->i])
	{
		while (shell->input[shell->i] && isspace(shell->input[shell->i]))
			shell->i++;
		if (!shell->input[shell->i])
			break ;
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
