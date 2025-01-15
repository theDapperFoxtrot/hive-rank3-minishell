#include "../../include/minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void add_token(t_ms *shell, t_token *new_token)
{
    t_token *current = shell->token;

    if (!new_token)
        return;
    if (!shell->token)
    {
        shell->token = new_token;
        return;
    }
    while (current->next)
        current = current->next;
    current->next = new_token;
}
void create_token(t_ms *shell, const char *value, int type)
{
    t_token *new_token = malloc(sizeof(t_token));
    if (!new_token)
        return;
    new_token->value = strdup(value);
    new_token->type = type;
    new_token->next = NULL;
    add_token(shell, new_token);
}


// Function to check if character is a special shell character
int is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == ';');
}
// void determine_token_type(const char *input, int *i, t_token **tokens)
// {
// }


void tokenize_input(t_ms *shell)
{
	int i = 0;
	char buffer[100000];
	int buf_i;
	int type;

	while (shell->input[i])
	{
		while (shell->input[i] && isspace(shell->input[i]))
			i++;
		if (!shell->input[i])
			break;
		if (is_operator(shell->input[i]))
		{
			buffer[0] = shell->input[i];
			buffer[1] = '\0';
			if (buffer[0] == '|')
			{
				type = TOKEN_PIPE;
				shell->pipe_count++;
			}
			else if (buffer[0] == '<')
			{
				if (shell->input[i + 1] == '<')
				{
					buffer[1] = '<';
					buffer[2] = '\0';
					i++;
					type = TOKEN_HERE_DOC;
				}
				else
					type = TOKEN_REDIR_IN;
			}
			else if (buffer[0] == '>')
			{
				if (shell->input[i + 1] == '>')
				{
					buffer[1] = '>';
					buffer[2] = '\0';
					i++;
					type = TOKEN_APPEND;
				}
				else
					type = TOKEN_REDIR_OUT;
			}
			create_token(shell, buffer, type);
			type = TOKEN_ARGS;
			shell->token_count++;
			i++;
			continue;
		}
		buf_i = 0;
		while (shell->input[i] && !isspace(shell->input[i]) && !is_operator(shell->input[i]))
		{
			if (shell->input[i] == '"' || shell->input[i] == '\'')
			{
				char quote = shell->input[i];
				buffer[buf_i++] = shell->input[i++];  // Include the opening quote
				while (shell->input[i] && shell->input[i] != quote)
					buffer[buf_i++] = shell->input[i++];
				if (shell->input[i] == quote)
					buffer[buf_i++] = shell->input[i++];  // Include the closing quote
			}
			else
				buffer[buf_i++] = shell->input[i++];
		}
		buffer[buf_i] = '\0';
		if (buf_i > 0)
			create_token(shell, buffer, type);
	}
}

// For debugging
void print_tokens(t_token *tokens)
{
	const char *type_names[] = {
		"ARGS", "PIPE", "REDIR_IN", "HERE_DOC", "REDIR_OUT", "TOKEN_APPEND"
	};

	while (tokens)
	{
		printf("Token: %-15s Type: %s\n",
			tokens->value,
			type_names[tokens->type]);
		tokens = tokens->next;
	}
}

// int main()
// {
// 	const char *input = "@ echo hello @world";
// 	t_token *tokens = NULL;

// 	tokenize_input(input);
// 	print_tokens(tokens);
// 	free_tokens(tokens);

// 	return 0;
// }
