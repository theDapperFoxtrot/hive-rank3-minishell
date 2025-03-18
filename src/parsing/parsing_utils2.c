#include "../../include/minishell.h"

char    *handle_expansions_quotes(t_ms *shell, const char *str)
{
    shell->exp.closing_quote = find_closing_quote(shell, str, str[shell->exp.i], shell->exp.i);
    if (shell->exp.closing_quote == -1)
    {
        free(shell->exp.result);
        return (ft_strdup(str));
    }
    shell->exp.quote_type = str[shell->exp.i];
	shell->exp.i++;
    while (shell->exp.i < shell->exp.closing_quote)
    {
        if (str[shell->exp.i] == '$' && shell->exp.quote_type == '\"')
        {
			if (handle_expansions_dollar_sign(shell, str) == -1)
				return (NULL);
			else
				continue ;
        }
        else
        {
            shell->exp.result = ft_realloc(shell->exp.result, shell->exp.j, shell->exp.j + 1);
            shell->exp.result[shell->exp.j++] = str[shell->exp.i++];
        }
    }
    shell->exp.i = shell->exp.closing_quote + 1;
    shell->exp.result = ft_realloc(shell->exp.result, shell->exp.j, shell->exp.j + 1);
    shell->exp.result[shell->exp.j] = '\0';
    return (shell->exp.result);
}

void    handle_token_args(t_ms *shell, t_command *cmd, t_token *token)
{
	char        *expanded_value;

	expanded_value = handle_expansions(shell, token->value);
	if (!expanded_value)
		return ;
	// if (expanded_value[0] == '\0')
	// {
	// 	free(expanded_value);
	// 	return ;
	// }
	add_argument(cmd, expanded_value);
	cmd->arg_count++;
	free(expanded_value);
}

t_command	*new_cmd_struct(t_ms *shell)
{
	t_command	*new_cmd;

	new_cmd = NULL;
	new_cmd = malloc(sizeof(t_command));
	if (!new_cmd)
		malloc_error(shell);
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
			malloc_error(shell);
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
