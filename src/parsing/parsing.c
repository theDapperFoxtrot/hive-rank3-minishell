#include "../../include/minishell.h"

void	add_argument(t_command *cmd, char *arg)
{
	int		i;
	char	**new_args;

	i = 0;
	if (cmd->args)
	{
		while (cmd->args[i] != NULL)
			i++;
	}
	new_args = ft_realloc(cmd->args, sizeof(char *) * \
	(i + 1), sizeof(char *) * (i + 2));
	if (!new_args)
		return ;
	cmd->args = new_args;
	cmd->args[i] = ft_strdup(arg);
	cmd->args[i + 1] = NULL;
	cmd->next = NULL;
}

int	find_closing_quote(t_ms *shell, const char *str, char quote_type, int start)
{
	int	i;

	i = start + 1;
	while (str[i] && str[i] != quote_type)
		i++;
	if (str[i] == quote_type)
		return (i);
	else
	{
		ft_putstr_fd("minishell: syntax error: Missing matching end quote symbol\n", 2);
		free(shell->exp.result);
		cleanup(shell, 1);
		exit(2);
	}
	// return (str[i] == quote_type) ? i : -1;
}

// Helper function to find closing brace
int	find_closing_brace(const char *str, int start)
{
	int	i;
	int brace_count;

	i = start;
	brace_count = 1;
	while (str[i] && brace_count > 0)
	{
		i++;
		if (str[i] == '{')
			brace_count++;
		else if (str[i] == '}')
			brace_count--;
	}
	if (brace_count == 0)
		return (i);
	else
		return (-1);
	// return (brace_count == 0) ? i : -1;
}

// Helper function to expand environment variables
char	*expand_env_var(t_ms *shell, const char *var_name, int with_braces)
{
	char *value = NULL;
	int i = 0;

	// Skip the '$' and optional '{' characters
	if (var_name[0] == '$')
		var_name++;
	if (with_braces && var_name[0] == '{')
		var_name++;

	// Handle special case for $?
	if (var_name[0] == '?' && (!with_braces || var_name[1] == '}'))
	{
		value = ft_itoa(shell->exit_code);
		var_name++;
		if (ft_isalnum(var_name[0]))
			value = ft_strjoin(value, var_name);
	}

	// Find the actual variable name length (up to '}' if in braces)
	int name_len = 0;
	while (var_name[name_len] &&
		((!with_braces && (ft_isalnum(var_name[name_len]) || var_name[name_len] == '_')) ||
			(with_braces && var_name[name_len] != '}')))
	{
		name_len++;
	}

	// Create temporary variable name for lookup
	char *temp_name = ft_substr(var_name, 0, name_len);
	if (!temp_name)
		return ft_strdup("");

	while (shell->env_list[i])
	{
		if (ft_strncmp(shell->env_list[i], temp_name, ft_strlen(temp_name)) == 0
			&& shell->env_list[i][ft_strlen(temp_name)] == '=')
		{
			value = ft_strdup(shell->env_list[i] + ft_strlen(temp_name) + 1);
			break;
		}
		i++;
	}

	free(temp_name);
	if (value)
		return (value);
	else
		return (ft_strdup(""));
	// return value ? value : ft_strdup("");
}

// Function to handle expansions within a string
char	*handle_expansions(t_ms *shell, const char *str)
{
	shell->exp.i = 0;
	shell->exp.j = 0;
	shell->exp.result = malloc(10000);  // Adjust size as needed
	if (!(shell->exp.result))
		return NULL;
	while (str[shell->exp.i])
	{
		if (str[shell->exp.i] == '\'' || str[shell->exp.i] == '\"')
			shell->exp.result = handle_expansions_quotes(shell, str);
		else if (str[shell->exp.i] == '$')
		{
			if (str[shell->exp.i + 1] == '{')
			{
				shell->exp.closing_brace = find_closing_brace(str, shell->exp.i + 1);
				if (shell->exp.closing_brace != -1)
				{
					shell->exp.var_name = ft_substr(str, shell->exp.i, shell->exp.closing_brace - shell->exp.i + 1);
					shell->exp.value = expand_env_var(shell, shell->exp.var_name, 1);
					ft_strlcpy(shell->exp.result + shell->exp.j, shell->exp.value, 10000 - shell->exp.j);
					shell->exp.j += ft_strlen(shell->exp.value);
					free(shell->exp.var_name);
					free(shell->exp.value);
					shell->exp.i = shell->exp.closing_brace + 1;
					continue;
				}
			}
			else
			{
				shell->exp.var_start = shell->exp.i + 1;
				shell->exp.var_len = 0;
				while (str[shell->exp.var_start + shell->exp.var_len] && \
				(ft_isalnum(str[shell->exp.var_start + shell->exp.var_len]) || \
				str[shell->exp.var_start + shell->exp.var_len] == '_' || \
				(shell->exp.var_len == 0 && str[shell->exp.var_start] == '?')))
					shell->exp.var_len++;
				if (shell->exp.var_len > 0)
				{
					shell->exp.var_name = ft_substr(str, shell->exp.var_start, shell->exp.var_len);
					shell->exp.value = expand_env_var(shell, shell->exp.var_name, 0);
					ft_strlcpy(shell->exp.result + shell->exp.j, shell->exp.value, 10000 - shell->exp.j);
					shell->exp.j += ft_strlen(shell->exp.value);
					free(shell->exp.var_name);
					free(shell->exp.value);
					shell->exp.i = shell->exp.var_start + shell->exp.var_len;
					continue;
				}
				else
					shell->exp.result[shell->exp.j++] = str[shell->exp.i++];
			}
		}
		else
			shell->exp.result[shell->exp.j++] = str[shell->exp.i++];
	}
	shell->exp.result[shell->exp.j] = '\0';
	return (shell->exp.result);
}

int	count_cmd_args(t_ms *shell, t_token *token)
{
	int	command_input_count;

	command_input_count = 0;
	while (token && token->type != TOKEN_PIPE)
	{
		if (token->type == TOKEN_ARGS)
			command_input_count++;
		else if (token->type == TOKEN_HERE_DOC)
			command_input_count++;
		else if (token->type == TOKEN_REDIR_IN)
			command_input_count = command_input_count + 2;
		else if (token->type == TOKEN_REDIR_OUT || token->type == TOKEN_APPEND)
			command_input_count = command_input_count + 2;
		if (token->next)
		{
			if (token->next->type == TOKEN_PIPE)
			{
				token = token->next;
				token = token->next;
				shell->next_start = token;
				break ;
			}
			token = token->next;
		}
		else
			break ;
	}
	return (command_input_count);
}

void parse_tokens(t_ms *shell)
{
	t_token		*token;
	t_command	*cmd;
	int			command_input_count;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return;
	ft_bzero(cmd, sizeof(t_command));
	shell->commands = cmd;
	token = shell->token;
	command_input_count = count_cmd_args(shell, token);
	if (command_input_count > 0)
	{
		cmd->command_input = (char **) malloc(sizeof(char *) * (command_input_count + 1));
		if (!cmd->command_input)
		{
			print_error("Error: malloc failed", shell, 1, 1);
			exit(shell->exit_code);
		}
	}
	cmd->command_input_index = 0;
	while (cmd->command_input_index < command_input_count)
	{
		cmd->command_input[cmd->command_input_index] = NULL;
		cmd->command_input_index++;
	}
	cmd->command_input_index = 0;
	while (token)
	{
		if (token->type == TOKEN_ARGS)
			handle_token_args(shell, cmd, token);
		else if (token->type == TOKEN_PIPE)
		{
			if (cmd->command_input)
				cmd->command_input[cmd->command_input_index] = NULL;
			cmd->next = handle_token_pipe(shell);
			cmd = cmd->next;
			cmd->command_input_index = 0;
			command_input_count = count_cmd_args(shell, shell->next_start);
			cmd->command_input = (char **) malloc(sizeof(char *) * (command_input_count + 1));
			if (!cmd->command_input)
			{
				print_error("Error: malloc failed", shell, 1, 1);
				exit(shell->exit_code);
			}
			cmd->next = NULL;
		}
		else if (token->type == TOKEN_REDIR_IN && token->next)
		{
			if (token->next->type == TOKEN_PIPE)
			{
				print_error("minishell: syntax error near unexpected token `|'", shell, 2, 0);
				shell->token_error = 1;
				return ;
			}
			handle_token_redir_in(shell, cmd, token);
			token = token->next;
		}
		else if (token->type == TOKEN_HERE_DOC && token->next)
		{
			handle_token_heredoc(shell, cmd, token);
			token = token->next;
		}
		else if (token->type == TOKEN_REDIR_OUT && token->next)
		{
			if (token->next->type == TOKEN_PIPE)
			{
				print_error("minishell: syntax error near unexpected token `|'", shell, 2, 0);
				shell->token_error = 1;
				return ;
			}
			handle_token_redir_out(shell, cmd, token);
			token = token->next;
		}
		else if (token->type == TOKEN_APPEND && token->next)
		{
			if (token->next->type == TOKEN_PIPE)
			{
				print_error("minishell: syntax error near unexpected token `|'", shell, 2, 0);
				shell->token_error = 1;
				return ;
			}
			handle_token_append(shell, cmd, token);
			token = token->next;
		}
		if (token->next == NULL)
		{
			if (cmd->command_input)
			cmd->command_input[cmd->command_input_index] = NULL;
			if ((ft_strncmp(token->value, ">", 1) == 0) || (ft_strncmp(token->value, "<", 1) == 0))
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
				free_tokens(shell);
				shell->token_error = 1;
				return ;
			}
		}
		token = token->next;
	}
}
