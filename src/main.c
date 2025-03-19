#include "../include/minishell.h"
#include <signal.h>
#include <termios.h>

int	g_signal = 0;

int	invalid_input(char *input)
{
	int	i;

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
	parse_tokens(shell);
	free_tokens(shell);
	if (shell->token_error)
		return ;
	if (g_signal == SIGINT)
	{
		shell->exit_code = 130;
		cleanup(shell, 0);
		g_signal = 0;
		return ;
	}
	check_command(shell, shell->commands);
	cleanup(shell, 0);
}

static void	init_shell(t_ms *shell, char **envp)
{
	ft_bzero(shell, sizeof(t_ms));
	shell->rd_in_count = 0;
	shell->rd_out_count = 0;
	shell->heredoc_line_count = 0;
	ft_bzero(&shell->exp, sizeof(t_exp));
	shell->exp.i = 0;
	create_env(shell, envp);
	shell->buffer = malloc(sizeof(char));
	if (!shell->buffer)
		malloc_error(shell);
	shell->buffer[0] = '\0';
}

void	input_loop(t_ms *shell)
{
	while (1)
	{
		if (init_signals() == 1)
			return ;
		shell->token_error = 0;
		if (isatty(fileno(stdin)))
			shell->input = readline("minishell> ");
		else
		{
			char *line;
			line = get_next_line(fileno(stdin));
			shell->input = ft_strtrim(line, "\n");
			free(line);
		}
		if (g_signal == SIGINT)
		{
			g_signal = 0;
			shell->exit_code = 130;
		}
		if (!shell->input)
		{
			// ft_putstr_fd("exit\n", 2);
			break ;
		}
		if (*(shell->input))
			add_history(shell->input);
		process_input(shell);
		rl_done = 1;
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	t_ms			shell;

	(void)argc;
	(void)argv;
	init_shell(&shell, envp);
	input_loop(&shell);
	rl_clear_history();
	cleanup(&shell, 1);
	return (shell.exit_code);
}
