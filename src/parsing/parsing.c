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
		ft_putstr_fd(\
			"minishell: syntax error: Missing matching end quote symbol\n", 2);
		free(shell->exp.result);
		cleanup(shell, 1);
		exit(2);
	}
}

// Helper function to find closing brace
int	find_closing_brace(const char *str, int start)
{
	int	i;
	int	brace_count;

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
}

char	*qmark_check(t_ms *shell, int with_braces)
{
	char	*temp_var;
	char	*temp;

	temp_var = shell->exp.var_name;
	if (temp_var[0] == '$')
		temp_var++;
	if (with_braces && temp_var[0] == '{')
		temp_var++;
	if (temp_var[0] == '?' && \
	(!with_braces || temp_var[1] == '}'))
	{
		shell->exp.value = ft_itoa(shell->exit_code);
		temp_var++;
		if (ft_isalnum(temp_var[0]))
		{
			temp = shell->exp.value;
			shell->exp.value = ft_strjoin(temp, temp_var);
			free(temp);
		}
	}
	return (shell->exp.value);
}

void	get_var_name_len(t_ms *shell, int with_braces)
{
	while (shell->exp.var_name[shell->exp_name_len] && \
	((!with_braces && \
	(ft_isalnum(shell->exp.var_name[shell->exp_name_len]) || \
	shell->exp.var_name[shell->exp_name_len] == '_')) || \
	(with_braces && \
	shell->exp.var_name[shell->exp_name_len] != '}')))
		shell->exp_name_len++;
}

void get_var_value(t_ms *shell)
{
    char *temp;
    char *joined;

    while (shell->env_list[shell->exp_i])
    {
        if (ft_strncmp(shell->env_list[shell->exp_i], shell->exp_temp_name,
            ft_strlen(shell->exp_temp_name)) == 0 &&
            shell->env_list[shell->exp_i][ft_strlen(shell->exp_temp_name)] == '=')
        {
            temp = shell->exp.value;
            // Store result of ft_strjoin
            joined = ft_strjoin(temp, shell->env_list[shell->exp_i] + 
                ft_strlen(shell->exp_temp_name) + 1);
            // Free the temporary value
            free(temp);
            // Use ft_strdup for the new value
            shell->exp.value = ft_strdup(shell->env_list[shell->exp_i] + 
                ft_strlen(shell->exp_temp_name) + 1);
            // Free joined string since we're using strdup instead
            free(joined);
            break;
        }
        shell->exp_i++;
    }
}

// Helper function to expand environment variables
void	expand_env_var(t_ms *shell, int with_braces)
{
	shell->exp_i = 0;
	shell->exp_name_len = 0;
	shell->exp.value = qmark_check(shell, with_braces);
	get_var_name_len(shell, with_braces);
	shell->exp_temp_name = ft_substr(shell->exp.var_name, 0, shell->exp_name_len);
	free(shell->exp.var_name);
	shell->exp.var_name = NULL;
	if (!shell->exp_temp_name)
		return ;
	get_var_value(shell);
	free(shell->exp_temp_name);
}

// Function to handle expansions within a string
char	*handle_expansions(t_ms *shell, const char *str)
{
	shell->exp.i = 0;
	shell->exp.j = 0;
	shell->exp.result = ft_strdup("");
	if (!(shell->exp.result))
		return (NULL);
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
					expand_env_var(shell, 1);
					shell->exp.result = ft_realloc(shell->exp.result, shell->exp.j, shell->exp.j + ft_strlen(shell->exp.value) + 1);
					ft_strlcpy(shell->exp.result + shell->exp.j, shell->exp.value, ft_strlen(shell->exp.value) + 1);
					shell->exp.j += ft_strlen(shell->exp.value);
					shell->exp.i = shell->exp.closing_brace + 1;
					continue ;
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
					expand_env_var(shell, 0);
					shell->exp.result = ft_realloc(shell->exp.result, shell->exp.j, shell->exp.j + ft_strlen(shell->exp.value) + 1);
					ft_strlcpy(shell->exp.result + shell->exp.j, shell->exp.value, ft_strlen(shell->exp.value) + 1);
					shell->exp.j += ft_strlen(shell->exp.value);
					shell->exp.i = shell->exp.var_start + shell->exp.var_len;
					continue ;
				}
				else
				{
					shell->exp.result = ft_realloc(shell->exp.result, shell->exp.j, shell->exp.j + 1);
					shell->exp.result[shell->exp.j++] = str[shell->exp.i++];
				}
			}
		}
		else
		{
			shell->exp.result = ft_realloc(shell->exp.result, shell->exp.j, shell->exp.j + 1);
			shell->exp.result[shell->exp.j++] = str[shell->exp.i++];
		}
	}
	free(shell->exp.value);
	shell->exp.value = NULL;
	free(shell->exp.var_name);
	shell->exp.var_name = NULL;
	shell->exp.result = ft_realloc(shell->exp.result, shell->exp.j, shell->exp.j + 1);
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

void	check_for_append_lm(t_ms *shell)
{
    // Ensure buffer has enough space
    shell->buffer = ft_realloc(shell->buffer, shell->buf_count, shell->buf_count + 1);
    if (!shell->buffer)
    {
        print_error("Error: malloc failed", shell, 1, 1);
        exit(shell->exit_code);
    }
    shell->buffer[1] = '>';
    shell->buffer[2] = '\0';
    shell->i++;
    shell->type = TOKEN_APPEND;
}

void	write_token_args_lm(t_ms *shell)
{
	shell->buf_i = 0;
	while (shell->pipe_rdl_tokens[shell->i] && \
	!isspace(shell->pipe_rdl_tokens[shell->i]) && \
	!is_operator(shell->pipe_rdl_tokens[shell->i]))
	{
		realloc_buffer(shell);
		if (shell->pipe_rdl_tokens[shell->i] == '"' || shell->pipe_rdl_tokens[shell->i] == '\'')
			if_quotes(shell);
		else
			shell->buffer[shell->buf_i++] = shell->pipe_rdl_tokens[shell->i++];
	}
	shell->buffer[shell->buf_i] = '\0';
	if (shell->buf_i > 0)
		create_token(shell);
	shell->new_buffer = ft_realloc(shell->buffer, ft_strlen(shell->buffer), 1);
	if (!shell->new_buffer)
	{
		print_error("Error: malloc failed", shell, 1, 1);
		exit(shell->exit_code);
	}
	shell->new_buffer[0] = '\0';
	shell->buffer = shell->new_buffer;
}


void	if_is_operator_lm(t_ms *shell)
{
	if (shell->buffer[0] == '|')
	{
		shell->type = TOKEN_PIPE;
		shell->pipe_count++;
	}
	else if (shell->buffer[0] == '<')
	{
		if (shell->pipe_rdl_tokens[shell->i + 1] == '<')
		{
			shell->buffer[1] = '<';
			shell->buffer[2] = '\0';
			shell->i++;
			shell->type = TOKEN_HERE_DOC;
		}
		else
		{
			shell->type = TOKEN_REDIR_IN;
			shell->rd_in_count++;
		}
	}
	else if (shell->buffer[0] == '>')
	{
		if (shell->pipe_rdl_tokens[shell->i + 1] == '>')
			check_for_append_lm(shell);
		else
		{
			shell->type = TOKEN_REDIR_OUT;
			shell->rd_out_count++;
		}
	}
}

int is_operator_true_lm(t_ms *shell)
{
    if (is_operator(shell->pipe_rdl_tokens[shell->i]))
    {
        // Allocate space for up to 2 chars plus null terminator
        shell->buffer = ft_realloc(shell->buffer, shell->buf_count, shell->buf_count + 2);
        if (!shell->buffer)
        {
            print_error("Error: malloc failed", shell, 1, 1);
            exit(shell->exit_code);
        }
        
        shell->buffer[0] = shell->pipe_rdl_tokens[shell->i];
        shell->buffer[1] = '\0';  // Ensure null termination
        
        if_is_operator_lm(shell);
        create_token(shell);
        shell->type = TOKEN_ARGS;
        shell->i++;
        
        // Allocate new buffer with proper size
        shell->new_buffer = ft_realloc(NULL, 0, 1);
        if (!shell->new_buffer)
        {
            print_error("Error: malloc failed", shell, 1, 1);
            exit(shell->exit_code);
        }
        
        free(shell->buffer);  // Free old buffer before reassignment
        shell->buffer = shell->new_buffer;
        shell->buffer[0] = '\0';
        shell->buf_count = 1;
        return (1);
    }
    return (0);
}

int	lead_pipe_check_lm(t_ms *shell, int lead_pipe)
{
	if (shell->pipe_rdl_tokens[shell->i] == '|' && lead_pipe)
	{
		if (shell->pipe_rdl_tokens[shell->i] == '|' && shell->pipe_rdl_tokens[shell->i + 1] == '|')
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `||'\n" \
				, 2);
			free_tokens(shell);
			return (1);
		}
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		shell->exit_code = 2;
		free_tokens(shell);
		return (1);
	}
	return (0);
}


void tokenize_input_lm(t_ms *shell)
{
    int lead_pipe;

    lead_pipe = 1;
    shell->i = 0;
    shell->buf_count = 1;
    shell->buffer = NULL;  // Initialize to NULL
    while (shell->pipe_rdl_tokens[shell->i])
    {
        while (shell->pipe_rdl_tokens[shell->i] &&
            ft_isspace(shell->pipe_rdl_tokens[shell->i]))
            shell->i++;
        if (lead_pipe_check_lm(shell, lead_pipe))
            return;
        if (!shell->pipe_rdl_tokens[shell->i])
            break;
        lead_pipe = 0;
        if (is_operator_true_lm(shell))
            continue;
        write_token_args_lm(shell);
        shell->buf_count = 1;
    }
    if (shell->buffer)
    {
        free(shell->buffer);
        shell->buffer = NULL;
    }
}

void print_tokens(t_token *token)
{
	while (token)
	{
		printf("Token: %s\n", token->value);
		token = token->next;
	}
}

int	pipe_syntax_check(t_ms *shell, t_token *token)
{
	if (token->next->type == TOKEN_PIPE)
	{
		print_error("minishell: syntax error near unexpected token `|'", shell, 2, 0);
		shell->exit_code = 2;
		shell->token_error = 1;
		return (1);
	}
	return (0);
}
t_command	*new_command(t_ms *shell, t_command *cmd, t_token *token, int command_input_count)
{
	t_command	*new_cmd;

	if (cmd && cmd->command_input && cmd->command_input_index >= 0)
		cmd->command_input[cmd->command_input_index] = NULL;
	new_cmd = new_cmd_struct(shell);
	cmd->next = new_cmd;
	cmd->next->command_input = cmd->command_input;
	cmd = new_cmd;
	cmd->command_input_index = 0;
	cmd->next = NULL;
	command_input_count = count_cmd_args(shell, shell->next_start);
	// printf("input count : %d\n", command_input_count);
	if (command_input_count >= 0)
	{
		cmd->command_input = (char **) malloc(sizeof(char *) * (command_input_count + 2));
		if (!cmd->command_input)
		{
			print_error("Error: malloc failed", shell, 1, 1);
			exit(shell->exit_code);
		}
		// printf("command_input_count: %p\n", cmd->command_input);
		while (cmd->command_input_index < command_input_count)
		{
			cmd->command_input[cmd->command_input_index] = NULL;
			cmd->command_input_index++;
		}
		cmd->command_input_index = 0;
	}
	else
		cmd->command_input = NULL;
	if (!token->next)
	{
		shell->pipe_rdl_tokens = readline("> ");
		add_history(shell->pipe_rdl_tokens);
		tokenize_input_lm(shell);
		free(shell->pipe_rdl_tokens);
		shell->pipe_rdl_tokens = NULL;
	}
	return (cmd);
}

int	setup_command_input_count(t_ms *shell, t_command *cmd, t_token *token, int command_input_count)
{
	command_input_count = count_cmd_args(shell, token);
	if (command_input_count > 0)
	{
		cmd->command_input = (char **) malloc(sizeof(char *) * (command_input_count + 2));
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
	return (command_input_count);
}

t_token	*check_token_redir(t_ms *shell, t_command *cmd, t_token *token)
{
	if (token->type == TOKEN_REDIR_IN && token->next)
		{
			if (pipe_syntax_check(shell, token))
				return (NULL);
			handle_token_redir_in(shell, cmd, token);
			token = token->next;
		}
		else if (token->type == TOKEN_HERE_DOC && token->next)
		{
			handle_token_heredoc(shell, cmd, token);
			if (g_signal == SIGINT)
				return (NULL);
			token = token->next;
		}
		else if (token->type == TOKEN_REDIR_OUT && token->next)
		{
			if (pipe_syntax_check(shell, token))
				return (NULL);
			handle_token_redir_out(shell, cmd, token);
			token = token->next;
		}
		else if (token->type == TOKEN_APPEND && token->next)
		{
			if (pipe_syntax_check(shell, token))
				return (NULL);
			handle_token_append(shell, cmd, token);
			token = token->next;
		}
		return (token);
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
	command_input_count = 0;
	command_input_count = setup_command_input_count(shell, cmd, token, command_input_count);
	cmd->command_input_index = 0;
	while (token)
	{
		if (token->type == TOKEN_ARGS)
			handle_token_args(shell, cmd, token);
		else if (token->type == TOKEN_PIPE)
			cmd = new_command(shell, cmd, token, command_input_count);
		else
		{
			token = check_token_redir(shell, cmd, token);
			if (g_signal == SIGINT)
				return ;
			if (!token)
				return ;
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
