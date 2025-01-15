#include "../include/minishell.h"

void *ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
    void    *new_ptr;

    if (!ptr)
        return (malloc(new_size));
    if (new_size == 0)
    {
        free(ptr);
        return (NULL);
    }
    new_ptr = malloc(new_size);
    if (!new_ptr)
    {
        free(ptr);
        return (NULL);
    }
    if (old_size > new_size)
        memcpy(new_ptr, ptr, new_size);
    else
        memcpy(new_ptr, ptr, old_size);
    free(ptr);
    return (new_ptr);
}

void add_argument(t_command *cmd, char *arg) {
    int i = 0;
    if (cmd->args) {
        while (cmd->args[i] != NULL)
            i++;
    }
    char **new_args = realloc(cmd->args, sizeof(char *) * (i + 2));  // +1 for new arg, +1 for NULL
    if (!new_args)
        return;
    cmd->args = new_args;
    cmd->args[i] = ft_strdup(arg);
    cmd->args[i + 1] = NULL;
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

void free_tokens(t_token *tokens)
{
	t_token *tmp;

	while (tokens)
	{
		tmp = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}

void parsing(t_ms *shell)
{
    tokenize_input(shell); // Tokenizes input and prepares for parsing
    parse_tokens(shell);   // Converts tokens into commands
	if (ft_strncmp(shell->input, "exit", 4) == 0)
	{
		shell->exit_code = 0;
		free(shell->input);
		free_tokens(shell->token);
		free_commands(shell->commands);
		free_env(shell);
		exit(shell->exit_code);
	}
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
