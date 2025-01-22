#include "../../include/minishell.h"

void    handle_token_args(t_ms *shell, t_command *cmd, t_token *token)
{
	char        *expanded_value;

	expanded_value = handle_expansions(shell, token->value);
	if (!expanded_value)
	{
		print_error("Error: malloc failed", shell, 1);
		exit(shell->exit_code);
	}
	add_argument(cmd, expanded_value);
	free(expanded_value);
}
t_command	*handle_token_pipe(t_ms *shell)
{
	t_command	*new_cmd;

	new_cmd = NULL;
	new_cmd = malloc(sizeof(t_command));
	if (!new_cmd)
	{
		print_error("Error: malloc failed", shell, 1);
		exit(shell->exit_code);
	}
	ft_bzero(new_cmd, sizeof(t_command));
	return (new_cmd);
}
void	handle_token_redir_in(t_ms *shell, t_command *cmd, t_token *token)
{
	char        *expanded_value;

	token = token->next;
	if (token && token->type == TOKEN_ARGS)
	{
		expanded_value = handle_expansions(shell, token->value);
		if (!expanded_value)
		{
			print_error("Error: malloc failed", shell, 1);
			exit(shell->exit_code);
		}
		cmd->input_file = ft_strdup(expanded_value);
		if (!cmd->input_file)
		{
			print_error("Error: malloc failed", shell, 1);
			exit(shell->exit_code);
		}
		free(expanded_value);
	}
}
void handle_token_redir_out(t_ms *shell, t_command *cmd, t_token *token)
{
	char        *expanded_value;


	token = token->next;
	if (token && token->type == TOKEN_ARGS)
	{
		expanded_value = handle_expansions(shell, token->value);
		if (!expanded_value)
		{
			print_error("Error: malloc failed", shell, 1);
			exit(shell->exit_code);
		}
		cmd->output_file = ft_strdup(expanded_value);
		if (!cmd->output_file)
		{
			print_error("Error: malloc failed", shell, 1);
			exit(shell->exit_code);
		}
		cmd->append_mode = (token->type == TOKEN_APPEND);
		free(expanded_value);
	}
}
