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

void	process_input(t_ms *shell)
{
	tokenize_input(shell);
	parse_tokens(shell); // <-- ADD EXPANSION; ${something}; single/double quotes
	clear_buffer(shell);
	free_tokens(shell);
	check_command(shell);
	free_commands(shell->commands);
	free(shell->input);
}

static void	init_shell(t_ms *shell, char **envp)
{
	ft_bzero(shell, sizeof(t_ms));
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
