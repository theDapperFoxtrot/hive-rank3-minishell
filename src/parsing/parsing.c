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

int	find_closing_quote(const char *str, char quote_type, int start)
{
	int	i;

	i = start + 1;
	while (str[i] && str[i] != quote_type)
		i++;
	if (str[i] == quote_type)
		return (i);
	else
		return (-1);
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

void	count_cmd_args(t_ms *shell, t_token *token)
{
	t_command	*cmd;

	cmd = shell->commands;
	while (token)
	{
		if (token->type == TOKEN_ARGS)
			cmd->command_input_count++;
		else if (token->type == TOKEN_HERE_DOC)
			cmd->command_input_count++;
		else if (token->type == TOKEN_REDIR_IN)
			cmd->command_input_count = cmd->command_input_count + 2;
		else if (token->type == TOKEN_REDIR_OUT || token->type == TOKEN_APPEND)
			cmd->command_input_count = cmd->command_input_count + 2;
		token = token->next;
	}
}

void parse_tokens(t_ms *shell)
{
	t_token		*token;
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return;
	ft_bzero(cmd, sizeof(t_command));
	shell->commands = cmd;
	token = shell->token;
	cmd->input_file = (char **) malloc(sizeof(char *) * shell->rd_in_count + 1);
	if (!cmd->input_file)
	{
		print_error("Error: malloc failed", shell, 1, 1);
		exit(shell->exit_code);
	}
	cmd->output_file = (char **) malloc(sizeof(char *) * shell->rd_out_count + 1);
	if (!cmd->output_file)
	{
		print_error("Error: malloc failed", shell, 1, 1);
		exit(shell->exit_code);
	}
	cmd->input_count = 0;
	cmd->output_count = 0;
	count_cmd_args(shell, token);
	cmd->command_input = (char **) malloc(sizeof(char *) * cmd->command_input_count + 1);
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
			cmd->input_count = 0;
			cmd->output_count = 0;
			cmd->free_input_count = 0;
			cmd->free_output_count = 0;
			cmd->command_input = (char **) malloc(sizeof(char *) * cmd->command_input_count + 1);
			cmd->command_input_count = 0;
			cmd->command_input_index = 0;
			cmd->input_file = (char **) malloc(sizeof(char *) * shell->rd_in_count + 1);
			if (!cmd->input_file)
			{
				print_error("Error: malloc failed", shell, 1, 1);
				exit(shell->exit_code);
			}
			cmd->output_file = (char **) malloc(sizeof(char *) * shell->rd_out_count + 1);
			if (!cmd->output_file)
			{
				print_error("Error: malloc failed", shell, 1, 1);
				exit(shell->exit_code);
			}
			cmd->next = NULL;
		}
		else if (token->type == TOKEN_REDIR_IN)
		{
			handle_token_redir_in(shell, cmd, token);
			token = token->next;
		}
		else if (token->type == TOKEN_HERE_DOC)
		{
			handle_token_heredoc(shell, cmd, token);
			token = token->next;
		}
		else if (token->type == TOKEN_REDIR_OUT)
		{
			handle_token_redir_out(shell, cmd, token);
			token = token->next;
		}
		else if (token->type == TOKEN_APPEND)
		{
			handle_token_append(shell, cmd, token);
			token = token->next;
		}
		if (!token->next)
		{
			if (cmd->command_input)
			cmd->command_input[cmd->command_input_index] = NULL;
		}
		token = token->next;
	}
}
