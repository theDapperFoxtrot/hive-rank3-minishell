#include "../include/minishell.h"
#include <signal.h>
#include <termios.h>

int g_signal = 0;

void	clear_buffer(t_ms *shell)
{
	int	i;

	i = 0;
	while (i < EXP_BUFFER_SIZE)
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
void print_redirections(t_command *cmd)
{
	while (cmd)
	{
		printf("New Command:\n");
		for (int i = 0; cmd->command_input[i]; i++)
			printf("%s\n", cmd->command_input[i]);
		printf("\n");
		cmd = cmd->next;
	}
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
	parse_tokens(shell);
	free_tokens(shell);
	// print_commands(shell->commands);
	// print_redirections(shell->commands);
	// clear_buffer(shell);
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
	{
		print_error("Error: malloc failed", shell, 1, 1);
		exit(shell->exit_code);
	}
	shell->buffer[0] = '\0';
}

void input_loop(t_ms *shell, struct termios *original_term)
{
	while (1)
	{
		shell->token_error = 0;
		shell->input = readline("minishell> ");
		if (g_signal == SIGINT)
        {
            g_signal = 0;
			shell->exit_code = 130;
        }
		if (!shell->input)
		{
			ft_putstr_fd("exit\n", 2);
			break ;
		}
		if (*(shell->input))
			add_history(shell->input);
		process_input(shell);
		tcsetattr(STDIN_FILENO, TCSANOW, original_term);
		rl_replace_line("", 0);
		rl_done = 1;
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	t_ms	shell;
	struct termios original_term;

	(void)argc;
	(void)argv;
	init_shell(&shell, envp);
	tcgetattr(STDIN_FILENO, &original_term);
	sig_init(&sig_handler_sigint);
	g_signal = 0;
	input_loop(&shell, &original_term);
	tcsetattr(STDIN_FILENO, TCSANOW, &original_term);
	rl_clear_history();
	cleanup(&shell, 1);
	if (shell.prev_pwd)
		free(shell.prev_pwd);
	return (shell.exit_code);
}
