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

void	process_input(t_ms *shell)
{
	tokenize_input(shell);
	parse_tokens(shell); // <-- ADD EXPANSION; ${something}; single/double quotes
	print_commands(shell->commands);
	clear_buffer(shell);
	free_tokens(shell);
	if (pipe(shell->fd) == -1)
	{
		print_error("Error: pipe failed", shell, 1);
		exit(shell->exit_code);
	}
	check_command(shell);
	free_commands(shell->commands);
	free(shell->input);
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
