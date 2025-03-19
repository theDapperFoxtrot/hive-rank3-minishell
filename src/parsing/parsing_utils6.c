#include "../../include/minishell.h"

void	checking_rd_out_lm(t_ms *shell)
{
	if (shell->pipe_rdl_tokens[shell->i + 1] == '>')
		check_for_append_lm(shell);
	else
	{
		shell->type = TOKEN_REDIR_OUT;
		shell->rd_out_count++;
	}
}

void	if_is_operator_lm(t_ms *shell)
{
	if (shell->buffer[0] == '|')
		checking_pipe_lm(shell);
	else if (shell->buffer[0] == '<')
		checking_rd_in_lm(shell);
	else if (shell->buffer[0] == '>')
		checking_rd_out_lm(shell);
}

int	is_operator_true_lm(t_ms *shell)
{
	if (is_operator(shell->pipe_rdl_tokens[shell->i]))
	{
		shell->buffer = ft_realloc(shell->buffer, shell->buf_count, \
						shell->buf_count + 2);
		if (!shell->buffer)
			malloc_error(shell);
		shell->buffer[0] = shell->pipe_rdl_tokens[shell->i];
		shell->buffer[1] = '\0';
		if_is_operator_lm(shell);
		create_token(shell);
		shell->type = TOKEN_ARGS;
		shell->i++;
		shell->new_buffer = ft_realloc(NULL, 0, 1);
		if (!shell->new_buffer)
			malloc_error(shell);
		free(shell->buffer);
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
		if (shell->pipe_rdl_tokens[shell->i] == '|' && \
			shell->pipe_rdl_tokens[shell->i + 1] == '|')
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

void	free_shell_buffer(t_ms *shell)
{
	free(shell->buffer);
	shell->buffer = NULL;
}
