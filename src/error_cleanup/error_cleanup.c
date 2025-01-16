#include "../../include/minishell.h"

void cleanup(t_ms *shell)
{
    if (shell->input)
        free(shell->input);
    if (shell->token)
        free_tokens(shell);
    if (shell->commands)
        free_commands(shell->commands);
    free_env(shell);
}

void free_commands(t_command *commands)
{
    t_command *tmp;

    while (commands)
    {
        tmp = commands->next;
        free_split(commands->args);
        free(commands->input_file);
        free(commands->output_file);
        free(commands);
        commands = tmp;
    }
}

void free_tokens(t_ms *shell)
{
    t_token *current;
    t_token *next;

    current = shell->token;
    while (current != NULL)
    {
        next = current->next;        // Store next pointer before freeing
        if (current->value != NULL)
        {
            free(current->value);
            current->value = NULL;
        }
        free(current);               // Free the token structure itself
        current = next;              // Move to next token
    }
}

