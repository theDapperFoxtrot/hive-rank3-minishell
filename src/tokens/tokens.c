#include "../../include/minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void add_token(t_ms *shell, t_token *new_token)
{
    t_token *current = shell->token;

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

void tokenize_input(t_ms *shell)
{
	shell->token = NULL;
	shell->i = 0;
	while (shell->input[shell->i])
	{
		//skip whitespace
		while (shell->input[shell->i] && isspace(shell->input[shell->i]))
			shell->i++;
		//if end of input, break
		if (!shell->input[shell->i])
			break;
		// check for special shell characters
		if (is_operator(shell->input[shell->i]))
		{
			shell->buffer[0] = shell->input[shell->i];
			shell->buffer[1] = '\0';
			// determine token type
			if_is_operator(shell);
			create_token(shell);
			shell->type = TOKEN_ARGS;
			shell->i++;
			continue;
		}
		write_token_args(shell);
	}
}

// For debugging
// void print_tokens(t_token *tokens)
// {
// 	const char *type_names[] = {
// 		"ARGS", "PIPE", "REDIR_IN", "HERE_DOC", "REDIR_OUT", "TOKEN_APPEND"
// 	};

// 	while (tokens)
// 	{
// 		printf("Token: %-15s Type: %s\n",
// 			tokens->value,
// 			type_names[tokens->type]);
// 		tokens = tokens->next;
// 	}
// }
