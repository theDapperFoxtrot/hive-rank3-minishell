#include "../../include/minishell.h"

void	cleanup(t_ms *shell, int clean_shell)
{
	if (shell->input)
	{
		free(shell->input);
		shell->input = NULL;
	}
	if (shell->token)
		free_tokens(shell);
	if (shell->commands)
		free_commands(shell->commands);
	if (clean_shell)
		free_env(shell);
}

void	free_commands(t_command *commands)
{
	t_command	*tmp;

	while (commands)
	{
		tmp = commands->next;
		free_split(commands->args);
		while (commands->free_input_count-- > 0)
			free(commands->input_file[commands->free_input_count]);
		free(commands->input_file);
		while (commands->free_output_count-- > 0)
			free(commands->output_file[commands->free_output_count]);
		free(commands->output_file);
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
