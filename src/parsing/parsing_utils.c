#include "../../include/minishell.h"

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
		cmd->input_file[cmd->input_count] = ft_strdup(expanded_value);
		if (!cmd->input_file)
		{
			print_error("Error: malloc failed", shell, 1, 1);
			exit(shell->exit_code);
		}
		cmd->redir_in = 1;
		free(expanded_value);
	}
	cmd->input_count++;
	cmd->free_input_count++;
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
		cmd->heredoc_line = ft_strjoin(cmd->heredoc_line, line);
		free(temp);
        free(line);
		i++;
	}
}

void	handle_token_heredoc(t_ms *shell, t_command *cmd, t_token *token)
{
	int			i;

	cmd->command_input[cmd->command_input_index] = ft_strdup(token->value);
	token = token->next;
	if (token && token->type == TOKEN_ARGS)
	{
		cmd->heredoc_delimiter = ft_strdup(token->value);
		if (!cmd->heredoc_delimiter)
		{
			print_error("Error: malloc failed", shell, 1, 1);
			exit(shell->exit_code);
		}
	}
	i = 0;
	cmd->heredoc_lines = (char **) malloc(sizeof(char *) * 100000);
	while (1)
	{
		cmd->heredoc_lines[i] = readline("> ");
		if (ft_strncmp(cmd->heredoc_lines[i], cmd->heredoc_delimiter, ft_strlen(cmd->heredoc_lines[i])) == 0)
			break ;
		i++;
	}
	cmd->heredoc_lines[i] = NULL;
	make_heredoc_one_line(shell, cmd);
	free(cmd->heredoc_lines);
	free(cmd->heredoc_delimiter);
	cmd->heredoc = 1;
	cmd->command_input_index++;
}


void handle_token_redir_out(t_ms *shell, t_command *cmd, t_token *token)
{
	char        *expanded_value;

	cmd->command_input[cmd->command_input_index] = ft_strdup(token->value);
	token = token->next;
	if (token && token->type == TOKEN_ARGS)
	{
		expanded_value = handle_expansions(shell, token->value);
		if (!expanded_value)
		{
			print_error("Error: malloc failed", shell, 1, 1);
			exit(shell->exit_code);
		}
		cmd->command_input[cmd->command_input_index + 1] = ft_strdup(expanded_value);
		cmd->output_file[cmd->output_count] = ft_strdup(expanded_value);
		if (!cmd->output_file)
		{
			print_error("Error: malloc failed", shell, 1, 1);
			exit(shell->exit_code);
		}
		cmd->redir_out = 1;
		free(expanded_value);
	}
	cmd->output_count++;
	cmd->free_output_count++;
	cmd->command_input_index += 2;
}

