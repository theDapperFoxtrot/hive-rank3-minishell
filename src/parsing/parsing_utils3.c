#include "../../include/minishell.h"

void handle_token_append(t_ms *shell, t_command *cmd, t_token *token)
{
	char        *expanded_value;

	cmd->command_input[cmd->command_input_index] = ft_strdup(token->value);
	if (!cmd->command_input[cmd->command_input_index])
		malloc_error(shell);
	token = token->next;
	if (token && token->type == TOKEN_ARGS)
	{
		expanded_value = handle_expansions(shell, token->value);
		if (!expanded_value)
			malloc_error(shell);
		cmd->command_input[cmd->command_input_index + 1] = ft_strdup(expanded_value);
		if (!cmd->command_input[cmd->command_input_index + 1])
			malloc_error(shell);
		cmd->append_mode = 1;
		free(expanded_value);
	}
	cmd->command_input_index += 2;
}

int	event(void)
{
	return (0);
}

int	handle_token_heredoc(t_ms *shell, t_command *cmd, t_token *token)
{
	int	i;

	if (pipe_syntax_check(shell, token))
		return (1);
	if (token && token->next && (token->next->type == TOKEN_REDIR_IN || token->next->type == TOKEN_REDIR_OUT))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putstr_fd(token->next->value, 2);
		ft_putstr_fd("'\n", 2);
		shell->exit_code = 2;
		shell->token_error = 1;
		return (1);
	}
	cmd->command_input[cmd->command_input_index] = ft_strdup(token->value);
	token = token->next;
	if (token && token->type == TOKEN_ARGS)
		setup_delim(shell, cmd, token);
	i = 0;
	check_signals(SIGINT, &sig_handler_heredoc);
	check_signals(SIGQUIT, &sig_handler_heredoc);
	shell->heredoc_line_count++;
	allocate_heredoc_lines(shell, cmd);
	rl_done = 0;
	rl_event_hook = event;
	i = 0;
	i = heredoc_loop(shell, cmd, i);
	if (g_signal == SIGINT)
		return (1);
	shell->heredoc_line_count = shell->heredoc_line_count + i;
	cmd->heredoc_lines[i] = NULL;
	make_heredoc_one_line(shell, cmd);
	cmd->heredoc = 1;
	cmd->command_input_index++;
	return (0);
}

void handle_token_redir_out(t_ms *shell, t_command *cmd, t_token *token)
{
	char        *expanded_value;

	cmd->command_input[cmd->command_input_index] = ft_strdup(token->value);
	if (!cmd->command_input[cmd->command_input_index])
		malloc_error(shell);
	token = token->next;
	if (token && token->type == TOKEN_ARGS)
	{
		expanded_value = handle_expansions(shell, token->value);
		if (!expanded_value)
			malloc_error(shell);
		cmd->command_input[cmd->command_input_index + 1] = ft_strdup(expanded_value);
		if (!cmd->command_input[cmd->command_input_index + 1])
			malloc_error(shell);
		cmd->redir_out = 1;
		free(expanded_value);
	}
	cmd->command_input_index += 2;
}

void	add_argument(t_command *cmd, char *arg)
{
	int		i;
	char	**new_args;

	i = 0;
	if (cmd->args)
	{
		while (cmd->args[i] != NULL)
			i++;
	}
	new_args = ft_realloc(cmd->args, sizeof(char *) * \
	(i + 1), sizeof(char *) * (i + 2));
	if (!new_args)
		return ;
	cmd->args = new_args;
	cmd->args[i] = ft_strdup(arg);
	cmd->args[i + 1] = NULL;
	cmd->next = NULL;
}
