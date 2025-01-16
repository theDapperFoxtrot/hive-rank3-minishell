#include "../../include/minishell.h"

int is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == ';');
}

void check_for_append(t_ms *shell)
{
    shell->buffer[1] = '>';
    shell->buffer[2] = '\0';
    shell->i++;
    shell->type = TOKEN_APPEND;
}

void if_is_operator(t_ms *shell)
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
			shell->buffer[1] = '<';
			shell->buffer[2] = '\0';
			shell->i++;
			shell->type = TOKEN_HERE_DOC;
		}
		else
			shell->type = TOKEN_REDIR_IN;
	}
	else if (shell->buffer[0] == '>')
	{
        if (shell->input[shell->i + 1] == '>')
            check_for_append(shell);
		else
			shell->type = TOKEN_REDIR_OUT;
	}
}

// void create_token(t_ms *shell)
// {
//     t_token *new_token;

//     new_token = malloc(sizeof(t_token));
//     if (!new_token)
//         return;

//     new_token->value = ft_strdup(shell->buffer);
//     if (!new_token->value)       // Check if strdup failed
//     {
//         free(new_token);         // Clean up if strdup fails
//         return;
//     }
    
//     new_token->type = shell->type;
//     new_token->next = NULL;      // Initialize next pointer to NULL
    
//     add_token(shell, new_token);
// }

void create_token(t_ms *shell)
{
	char *value;
	int type;

	value = shell->buffer;
	type = shell->type;
    t_token *new_token = malloc(sizeof(t_token));
    if (!new_token)
        return;
    new_token->value = ft_strdup(value);
	if (!new_token->value)
	{
		//free the whole token list
		// need error/cleanup function
		free(new_token);
		return;
	}
    new_token->type = type;
    new_token->next = NULL;
    add_token(shell, new_token);
}

// Function to check if character is a special shell character
void write_token_args(t_ms *shell)
{

		shell->buf_i = 0;
		while (shell->input[shell->i] && !isspace(shell->input[shell->i]) && !is_operator(shell->input[shell->i]))
		{
			if (shell->input[shell->i] == '"' || shell->input[shell->i] == '\'')
			{
				char quote = shell->input[shell->i];
				shell->buffer[shell->buf_i++] = shell->input[shell->i++];  // Include the opening quote
				while (shell->input[shell->i] && shell->input[shell->i] != quote)
					shell->buffer[shell->buf_i++] = shell->input[shell->i++];
				if (shell->input[shell->i] == quote)
					shell->buffer[shell->buf_i++] = shell->input[shell->i++];  // Include the closing quote
			}
			else
				shell->buffer[shell->buf_i++] = shell->input[shell->i++];
		}
		shell->buffer[shell->buf_i] = '\0';
		if (shell->buf_i > 0)
			create_token(shell);
}
