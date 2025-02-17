#include "../../include/minishell.h"

void handle_token_append(t_ms *shell, t_command *cmd, t_token *token)
{
	char        *expanded_value;

	cmd->command_input[cmd->command_input_index] = ft_strdup(token->value);
	if (!cmd->command_input[cmd->command_input_index])
	{
		print_error("Error: malloc failed", shell, 1, 1);
		exit(shell->exit_code);
	}
	token = token->next;
	if (token && token->type == TOKEN_ARGS)
	{
		expanded_value = handle_expansions(shell, token->value);
		if (!expanded_value)
		{
			print_error("Error: malloc failed", shell, 1, 1);
			exit(shell->exit_code);
		}
		cmd->command_input[cmd->command_input_index + 1] = ft_strdup(expanded_value);
		if (!cmd->command_input[cmd->command_input_index + 1])
		{
			print_error("Error: malloc failed", shell, 1, 1);
			exit(shell->exit_code);
		}
		cmd->append_mode = 1;
		free(expanded_value);
	}
	cmd->command_input_index += 2;
}
