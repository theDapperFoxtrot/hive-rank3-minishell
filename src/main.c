#include "../include/minishell.h"

void parsing(t_ms *shell)
{
	tokenize_input(shell);
	print_tokens(shell->token);
}

static void	init_shell(t_ms *shell, char **envp)
{
	create_env(shell, envp);
	shell->exit_code = 0;
}

int	main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	t_ms shell;
	ft_bzero(&shell, sizeof(t_ms));
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
		parsing(&shell);
	}
	rl_clear_history();
	free_env(&shell);
	return (shell.exit_code);
}
