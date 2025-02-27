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

void	get_var_value(t_ms *shell)
{
	while (shell->env_list[shell->exp_i])
	{
		if (ft_strncmp(shell->env_list[shell->exp_i], shell->exp_temp_name, \
		ft_strlen(shell->exp_temp_name)) == 0 && \
		shell->env_list[shell->exp_i][ft_strlen(shell->exp_temp_name)] == '=')
		{
			shell->exp.value = \
			ft_strdup(shell->env_list[shell->exp_i] + \
			ft_strlen(shell->exp_temp_name) + 1);
			break ;
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
	free(shell->exp.var_name);  // Free var_name here
	shell->exp.var_name = NULL;
	if (!shell->exp_temp_name)
		return ;
	get_var_value(shell);
	free(shell->exp_temp_name);
	// if (shell->exp.value)
	// {
	// 	ft_strdup(shell->exp.value);
	// 	free(shell->exp.value);  // Free exp.value here
	// 	shell->exp.value = NULL;
	// }
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
	shell->buffer[1] = '>';
	shell->buffer[2] = '\0';
	shell->i++;
	shell->type = TOKEN_APPEND;
}

void	write_token_args_lm(t_ms *shell)
{
	char	quote;

	shell->buf_i = 0;
	while (shell->pipe_rdl_tokens[shell->i] && \
	!isspace(shell->pipe_rdl_tokens[shell->i]) && \
	!is_operator(shell->pipe_rdl_tokens[shell->i]))
	{
		if (shell->pipe_rdl_tokens[shell->i] == '"' || shell->pipe_rdl_tokens[shell->i] == '\'')
		{
			quote = shell->pipe_rdl_tokens[shell->i];
			shell->buffer[shell->buf_i++] = shell->pipe_rdl_tokens[shell->i++];
			while (shell->pipe_rdl_tokens[shell->i] && shell->pipe_rdl_tokens[shell->i] != quote)
				shell->buffer[shell->buf_i++] = shell->pipe_rdl_tokens[shell->i++];
			if (shell->pipe_rdl_tokens[shell->i] == quote)
				shell->buffer[shell->buf_i++] = shell->pipe_rdl_tokens[shell->i++];
		}
		else
			shell->buffer[shell->buf_i++] = shell->pipe_rdl_tokens[shell->i++];
	}
	shell->buffer[shell->buf_i] = '\0';
	if (shell->buf_i > 0)
		create_token(shell);
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


void	tokenize_input_lm(t_ms *shell)
{
	int		lead_pipe;

	lead_pipe = 1;
	shell->i = 0;
	while (shell->pipe_rdl_tokens[shell->i])
	{
		while (shell->pipe_rdl_tokens[shell->i] && ft_isspace(shell->pipe_rdl_tokens[shell->i]))
			shell->i++;
		if (shell->pipe_rdl_tokens[shell->i] == '|' && lead_pipe)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
			free_tokens(shell);
			return ;
		}
		if (!shell->pipe_rdl_tokens[shell->i])
			break ;
		lead_pipe = 0;
		if (is_operator(shell->pipe_rdl_tokens[shell->i]))
		{
			shell->buffer[0] = shell->pipe_rdl_tokens[shell->i];
			shell->buffer[1] = '\0';
			if_is_operator_lm(shell);
			create_token(shell);
			shell->type = TOKEN_ARGS;
			shell->i++;
			continue ;
		}
		write_token_args_lm(shell);
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
		if ((ft_strlen(token->value) > 255) && token->type == TOKEN_ARGS)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(token->value, 2);
			ft_putstr_fd(": command not found\n", 2);
			shell->exit_code = 1;
			shell->token_error = 1;
			cleanup(shell, 0);
			return ;
		}
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
			if (!token->next)
			{
				shell->pipe_rdl_tokens = readline("> ");
				tokenize_input_lm(shell);
				free(shell->pipe_rdl_tokens);
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
			if (g_signal == SIGINT)
				break ;
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



// char *expand_env_var(t_ms *shell, int with_braces)
// {
// 	char *result;

// 	shell->exp_value = NULL;
// 	shell->exp_name_len = 0;
// 	/* 
// 	 * qmark_check uses shell->exp.var_name but does not change its address.
// 	 * That way we can later free the original pointer.
// 	 */
// 	shell->exp_value = qmark_check(shell, with_braces);
// 	get_var_name_len(shell, with_braces);
// 	/* 
// 	 * Create a temporary copy of the variable name.
// 	 * Then free the original allocation so we don’t leak.
// 	 */
// 	shell->exp_temp_name = ft_substr(shell->exp.var_name, 0, shell->exp_name_len);
// 	free(shell->exp.var_name);
// 	shell->exp.var_name = NULL;
	
// 	if (!shell->exp_temp_name)
// 		return (ft_strdup(""));
		
// 	get_var_value(shell);
// 	free(shell->exp_temp_name);
// 	shell->exp_temp_name = NULL;
	
// 	if (shell->exp_value)
// 	{
// 		result = ft_strdup(shell->exp_value);
// 		free(shell->exp_value);
// 		shell->exp_value = NULL;
// 		return result;
// 	}
// 	return ft_strdup("");
// }

// char *handle_expansions(t_ms *shell, const char *str)
// {
// 	shell->exp.i = 0;
// 	shell->exp.j = 0;
// 	shell->exp.result = ft_strdup("");
// 	if (!shell->exp.result)
// 		return NULL;
// 	while (str[shell->exp.i])
// 	{
// 		if (str[shell->exp.i] == '\'' || str[shell->exp.i] == '\"')
// 		{
// 			shell->exp.result = handle_expansions_quotes(shell, str);
// 		}
// 		else if (str[shell->exp.i] == '$')
// 		{
// 			if (str[shell->exp.i + 1] == '{')
// 			{
// 				shell->exp.closing_brace = find_closing_brace(str, shell->exp.i + 1);
// 				if (shell->exp.closing_brace != -1)
// 				{
// 					shell->exp.var_name = ft_substr(str, shell->exp.i,
// 						shell->exp.closing_brace - shell->exp.i + 1);
// 					shell->exp.value = expand_env_var(shell, 1);
// 					shell->exp.result = ft_realloc(shell->exp.result, shell->exp.j,
// 						shell->exp.j + ft_strlen(shell->exp.value) + 1);
// 					ft_strlcpy(shell->exp.result + shell->exp.j, shell->exp.value,
// 						ft_strlen(shell->exp.value) + 1);
// 					shell->exp.j += ft_strlen(shell->exp.value);
// 					shell->exp.i = shell->exp.closing_brace + 1;
// 					continue;
// 				}
// 			}
// 			else
// 			{
// 				shell->exp.var_start = shell->exp.i + 1;
// 				shell->exp.var_len = 0;
// 				while (str[shell->exp.var_start + shell->exp.var_len] &&
// 					  (ft_isalnum(str[shell->exp.var_start + shell->exp.var_len]) ||
// 					   str[shell->exp.var_start + shell->exp.var_len] == '_' ||
// 					   (shell->exp.var_len == 0 && str[shell->exp.var_start] == '?')))
// 				{
// 					shell->exp.var_len++;
// 				}
// 				if (shell->exp.var_len > 0)
// 				{
// 					shell->exp.var_name = ft_substr(str, shell->exp.var_start, shell->exp.var_len);
// 					shell->exp.value = expand_env_var(shell, 0);
// 					shell->exp.result = ft_realloc(shell->exp.result, shell->exp.j,
// 						shell->exp.j + ft_strlen(shell->exp.value) + 1);
// 					ft_strlcpy(shell->exp.result + shell->exp.j, shell->exp.value,
// 						ft_strlen(shell->exp.value) + 1);
// 					shell->exp.j += ft_strlen(shell->exp.value);
// 					shell->exp.i = shell->exp.var_start + shell->exp.var_len;
// 					continue;
// 				}
// 				else
// 				{
// 					shell->exp.result = ft_realloc(shell->exp.result, shell->exp.j, shell->exp.j + 1);
// 					shell->exp.result[shell->exp.j++] = str[shell->exp.i++];
// 				}
// 			}
// 		}
// 		else
// 		{
// 			shell->exp.result = ft_realloc(shell->exp.result, shell->exp.j, shell->exp.j + 1);
// 			shell->exp.result[shell->exp.j++] = str[shell->exp.i++];
// 		}
// 	}
// 	shell->exp.result = ft_realloc(shell->exp.result, shell->exp.j, shell->exp.j + 1);
// 	shell->exp.result[shell->exp.j] = '\0';
// 	return shell->exp.result;
// }

// char *handle_expansions_quotes(t_ms *shell, const char *str)
// {
// 	shell->exp.closing_quote = find_closing_quote(shell, str, str[shell->exp.i], shell->exp.i);
// 	if (shell->exp.closing_quote == -1)
// 	{
// 		free(shell->exp.result);
// 		return ft_strdup(str);
// 	}
// 	shell->exp.quote_type = str[shell->exp.i];
// 	shell->exp.i++;
// 	while (shell->exp.i < shell->exp.closing_quote)
// 	{
// 		if (str[shell->exp.i] == '$' && shell->exp.quote_type == '\"')
// 		{
// 			if (str[shell->exp.i + 1] == '{')
// 			{
// 				shell->exp.closing_brace = find_closing_brace(str, shell->exp.i + 1);
// 				if (shell->exp.closing_brace != -1)
// 				{
// 					shell->exp.var_name = ft_substr(str, shell->exp.i,
// 						shell->exp.closing_brace - shell->exp.i + 1);
// 					shell->exp.value = expand_env_var(shell, 1);
// 					shell->exp.result = ft_realloc(shell->exp.result, shell->exp.j,
// 						shell->exp.j + ft_strlen(shell->exp.value) + 1);
// 					ft_strlcpy(shell->exp.result + shell->exp.j, shell->exp.value,
// 						ft_strlen(shell->exp.value) + 1);
// 					shell->exp.j += ft_strlen(shell->exp.value);
// 					shell->exp.i = shell->exp.closing_brace + 1;
// 					continue;
// 				}
// 			}
// 			else
// 			{
// 				shell->exp.var_start = shell->exp.i + 1;
// 				shell->exp.var_len = 0;
// 				while (str[shell->exp.var_start + shell->exp.var_len] &&
// 					  (ft_isalnum(str[shell->exp.var_start + shell->exp.var_len]) ||
// 					   str[shell->exp.var_start + shell->exp.var_len] == '_' ||
// 					   (shell->exp.var_len == 0 && str[shell->exp.var_start] == '?')))
// 				{
// 					shell->exp.var_len++;
// 				}
// 				if (shell->exp.var_len > 0)
// 				{
// 					shell->exp.var_name = ft_substr(str, shell->exp.var_start, shell->exp.var_len);
// 					shell->exp.value = expand_env_var(shell, 0);
// 					shell->exp.result = ft_realloc(shell->exp.result, shell->exp.j,
// 						shell->exp.j + ft_strlen(shell->exp.value) + 1);
// 					ft_strlcpy(shell->exp.result + shell->exp.j, shell->exp.value,
// 						ft_strlen(shell->exp.value) + 1);
// 					shell->exp.j += ft_strlen(shell->exp.value);
// 					shell->exp.i = shell->exp.var_start + shell->exp.var_len;
// 					continue;
// 				}
// 			}
// 		}
// 		shell->exp.result[shell->exp.j++] = str[shell->exp.i++];
// 	}
// 	shell->exp.i = shell->exp.closing_quote + 1;
// 	shell->exp.result = ft_realloc(shell->exp.result, shell->exp.j, shell->exp.j + 1);
// 	shell->exp.result[shell->exp.j] = '\0';
// 	return shell->exp.result;
// }