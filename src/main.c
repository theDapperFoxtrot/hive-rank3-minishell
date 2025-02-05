#include "../include/minishell.h"

void	clear_buffer(t_ms *shell)
{
	int	i;

	i = 0;
	while (i < 10000)
	{
		shell->buffer[i] = '\0';
		i++;
	}
}
void print_commands(t_command *cmd)
{
	int	i;
	int j;

	j = 0;
	i = 0;
	while (cmd != NULL)
	{
		printf("----[Structure %d]----\n", j);
		while (cmd->args[i])
		{
			printf("[%d]%s\n", i, cmd->args[i]);
			i++;
		}
		i = 0;
		j++;
		cmd = cmd->next;
	}
	printf("----[End of Structure]----\n");
}

int	invalid_input(char *input)
{
    int i;

    i = 0;
    while (input[i])
    {
        if (input[i] != ' ' \
		&& input[i] != '\t' \
		&& input[i] != '\n' \
		&& input[i] != '\v' \
		&& input[i] != '\f' \
		&& input[i] != '\r')
            return (0);
        i++;
    }
    return (1);
}

void	process_input(t_ms *shell)
{
	if (invalid_input(shell->input))
		return ;
	tokenize_input(shell);
	if (!shell->token)
		return ;
	parse_tokens(shell); // <-- ADD EXPANSION; ${something}; single/double quotes
	// print_commands(shell->commands);
	clear_buffer(shell);
	check_command(shell);
	cleanup(shell, 0);
}

static void	init_shell(t_ms *shell, char **envp)
{
	ft_bzero(shell, sizeof(t_ms));
	ft_bzero(&shell->exp, sizeof(t_exp));
	shell->exp.i = 0;
	create_env(shell, envp);
}


int	main(int argc, char *argv[], char *envp[])
{
	t_ms	shell;

	(void)argc;
	(void)argv;
	init_shell(&shell, envp);
	while (1)
	{
		shell.input = readline("minishell> ");
		if (!shell.input)
		{
			printf("exit\n");
			break ;
		}
		if (*(shell.input))
			add_history(shell.input);
		process_input(&shell);
	}
	rl_clear_history();
	free_env(&shell);
	return (shell.exit_code);
}
