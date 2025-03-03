#include "../../include/minishell.h"

void malloc_error(t_ms *shell)
{
	print_error("Error: malloc failed", shell, 1, 1);
	exit(shell->exit_code);
}

void    handle_token_args(t_ms *shell, t_command *cmd, t_token *token)
{
	char        *expanded_value;

	expanded_value = handle_expansions(shell, token->value);
	if (!expanded_value)
	{
		print_error("Error: malloc failed", shell, 1, 1);
		exit(shell->exit_code);
	}
	if (expanded_value[0] == '\0')
	{
		free(expanded_value);
		return ;
	}
	add_argument(cmd, expanded_value);
	cmd->arg_count++;
	free(expanded_value);
}
t_command	*handle_token_pipe(t_ms *shell)
{
	t_command	*new_cmd;

	new_cmd = NULL;
	new_cmd = malloc(sizeof(t_command));
	if (!new_cmd)
	{
		print_error("Error: malloc failed", shell, 1, 1);
		exit(shell->exit_code);
	}
	ft_bzero(new_cmd, sizeof(t_command));
	return (new_cmd);
}
void	handle_token_redir_in(t_ms *shell, t_command *cmd, t_token *token)
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
		{
			print_error("minishell: syntax error", shell, 1, 1);
			exit(shell->exit_code);
		}
		cmd->command_input[cmd->command_input_index + 1] = ft_strdup(expanded_value);
		if (!cmd->command_input[cmd->command_input_index + 1])
			malloc_error(shell);
		cmd->redir_in = 1;
		free(expanded_value);
	}
	cmd->command_input_index += 2;
}

void	make_heredoc_one_line(t_ms *shell, t_command *cmd)
{
	int		i;
	char	*line;
	char	*temp;

	i = 0;
	cmd->heredoc_line = ft_strdup("");
	while(cmd->heredoc_lines[i])
	{
		line = ft_strjoin(cmd->heredoc_lines[i], "\n");
		if (!line)
		{
			print_error("Error: malloc failed", shell, 1, 1);
			free_split(cmd->heredoc_lines);
			exit(shell->exit_code);
		}
		free(cmd->heredoc_lines[i]);
		temp = cmd->heredoc_line;
		cmd->heredoc_line = ft_strjoin(temp, line);
		free(temp);
        free(line);
		i++;
	}
	free(cmd->heredoc_lines);
	cmd->heredoc_lines = NULL;
}

int	event(void)
{
	return (0);
}
void	setup_delim(t_ms *shell, t_command *cmd, t_token *token)
{
	cmd->heredoc_delimiter = ft_strdup(token->value);
	if (!cmd->heredoc_delimiter)
		malloc_error(shell);
}

void	allocate_heredoc_lines(t_ms *shell, t_command *cmd)
{
	cmd->heredoc_lines = (char **) malloc(sizeof(char *) * 2);
	if (!cmd->heredoc_lines)
	{
		print_error("Error: malloc failed", shell, 1, 1);
		exit(shell->exit_code);
	}
	cmd->heredoc_lines[0] = NULL;
}

int	heredoc_lines_err(t_ms *shell, t_command *cmd, int i)
{
	if (!cmd->heredoc_lines[i])
	{
		ft_putstr_fd("minishell: warning: here-document at line ", 2);
		ft_putnbr_fd(shell->heredoc_line_count, 2);
		ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
		ft_putstr_fd(cmd->heredoc_delimiter, 2);
		ft_putstr_fd("')", 2);
		ft_putstr_fd("\n", 2);
		return (1);
	}
	return (0);
}

int	heredoc_loop(t_ms *shell, t_command *cmd, int i)
{
	while (1)
	{
		sig_heredoc(&sig_handler_heredoc);
		cmd->heredoc_lines = ft_realloc(cmd->heredoc_lines, sizeof(char *) * i, sizeof(char *) * (i + 1));
		if (!cmd->heredoc_lines)
		{
			print_error("Error: malloc failed", shell, 1, 1);
			exit(shell->exit_code);
		}
		cmd->heredoc_lines[i] = readline("> ");
		if (g_signal == SIGINT)
			break ;
		if (heredoc_lines_err(shell, cmd, i))
			break ;
		if (ft_strncmp(cmd->heredoc_lines[i], cmd->heredoc_delimiter, ft_strlen(cmd->heredoc_lines[i])) == 0)
		break ;
		i++;
	}
	return (i);
}

void	handle_token_heredoc(t_ms *shell, t_command *cmd, t_token *token)
{
	int	i;

	cmd->command_input[cmd->command_input_index] = ft_strdup(token->value);
	token = token->next;
	if (token && token->type == TOKEN_ARGS)
		setup_delim(shell, cmd, token);
	i = 0;
	sig_heredoc(&sig_handler_heredoc);
	shell->heredoc_line_count++;
	allocate_heredoc_lines(shell, cmd);
	i = 0;
	rl_done = 0;
	rl_event_hook = event;
	i = heredoc_loop(shell, cmd, i);
	if (g_signal == SIGINT)
		return ;
	shell->heredoc_line_count = shell->heredoc_line_count + i;
	cmd->heredoc_lines[i] = NULL;
	make_heredoc_one_line(shell, cmd);
	cmd->heredoc = 1;
	cmd->command_input_index++;
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
