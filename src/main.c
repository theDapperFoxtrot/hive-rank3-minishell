#include "../include/minishell.h"


void add_argument(t_command *cmd, char *arg)
{
	int i;

	i = 0;
	if (cmd->args) {
		while (cmd->args[i] != NULL)
			i++;
	}
	char **new_args = ft_realloc(cmd->args, sizeof(char *) * (i + 1), sizeof(char *) * (i + 2));  // +1 for new arg, +1 for NULL
	if (!new_args)
		return;
	cmd->args = new_args;
	cmd->args[i] = ft_strdup(arg);
    // printf("%s\n", cmd->args[i]);
	cmd->args[i + 1] = NULL;
	cmd->next = NULL;
}

void parse_tokens(t_ms *shell)
{
	t_token		*token;
	t_command	*cmd;
	t_command	*prev_cmd = NULL;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return ;
	ft_bzero(cmd, sizeof(t_command));
	cmd->args = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->append_mode = 0;
	cmd->next = NULL;
	shell->commands = cmd;
	token = shell->token;
	while (token)
	{
		if (token->type == TOKEN_ARGS)
			add_argument(cmd, token->value);
		else if (token->type == TOKEN_PIPE)
		{
			prev_cmd = cmd;
			cmd = malloc(sizeof(t_command));
			if (!cmd)
				return ;
			ft_bzero(cmd, sizeof(t_command));
			cmd->args = NULL;
			cmd->input_file = NULL;
			cmd->output_file = NULL;
			cmd->append_mode = 0;
			cmd->next = NULL;
			prev_cmd->next = cmd;
		}
		else if (token->type == TOKEN_REDIR_IN)
		{
			token = token->next;
			if (token && token->type == TOKEN_ARGS)
				cmd->input_file = ft_strdup(token->value);
		}
		else if (token->type == TOKEN_REDIR_OUT)
		{
			token = token->next;
			if (token && token->type == TOKEN_ARGS)
				cmd->output_file = ft_strdup(token->value);
		}
		token = token->next;
	}
}
void clear_buffer(t_ms *shell)
{
	for (int i = 0; i < 10000; i++) {
	shell->buffer[i] = '\0';
	}
}

void parsing(t_ms *shell)
{
	tokenize_input(shell); // Tokenizes input and prepares for parsing
	// print_tokens(shell->token); // For debugging
	parse_tokens(shell);   // Converts tokens into commands
	clear_buffer(shell);
	free_tokens(shell); // Free the tokens list
	check_command(shell);  // Checks if the command is a builtin or external command
	free_commands(shell->commands); // Free the commands list
	shell->commands = NULL; // Ensure the commands pointer is reset
	free(shell->input);
}

static void	init_shell(t_ms *shell, char **envp)
{
	ft_bzero(shell, sizeof(t_ms));
	create_env(shell, envp);
}

int	main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	t_ms shell;
	init_shell(&shell, envp);
	while (1)
	{
		// clear out all the stuff here
		shell.input = readline("minishell> ");
		if (!shell.input)
		{
			printf("exit\n");
			break ;
		}
		if (*(shell.input))
			add_history(shell.input);
		parsing(&shell);
	}
	rl_clear_history();
	free_env(&shell);
	return (shell.exit_code);
}
