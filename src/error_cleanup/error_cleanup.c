#include "../../include/minishell.h"

void	cleanup(t_ms *shell, int clean_shell)
{
    if (shell->input)
    {
        free(shell->input);
        shell->input = NULL;
    }
    if (shell->token)
    {
        free_tokens(shell);
        shell->token = NULL;
    }
    if (shell->commands)
    {
        free_commands(shell->commands);
        shell->commands = NULL;
    }
    if (clean_shell)
        free_env(shell);
}

void	free_commands(t_command *commands)
{
	t_command	*tmp;

	while (commands)
	{
		tmp = commands->next;
		if (commands->args)
			free_split(commands->args);
		if (commands->heredoc_lines)
			free(commands->heredoc_lines);
		if (commands->heredoc_delimiter)
			free(commands->heredoc_delimiter);
		if (commands->heredoc_line)
			free(commands->heredoc_line);
		if (commands->command_input)
			free_split(commands->command_input);
		if (commands)
			free(commands);
		commands = tmp;
	}
	commands = NULL;
}

void	free_tokens(t_ms *shell)
{
	t_token	*current;
	t_token	*next;

	current = shell->token;
	while (current != NULL)
	{
		next = current->next;
		if (current->value != NULL)
		{
			free(current->value);
			current->value = NULL;
		}
		free(current);
		current = next;
	}
	shell->token = NULL;
}
