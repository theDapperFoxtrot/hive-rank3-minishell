#include "../../include/minishell.h"

void	realloc_buffer(t_ms *shell)
{
	shell->new_buffer = ft_realloc(shell->buffer, shell->buf_count, \
		shell->buf_count + 1);
	if (!shell->new_buffer)
	{
		print_error("Error: malloc failed", shell, 1, 1);
		exit(shell->exit_code);
	}
	shell->buffer = shell->new_buffer;
	shell->buf_count++;
}

void	if_quotes(t_ms *shell)
{
	char	quote;

	quote = shell->input[shell->i];
	realloc_buffer(shell);
	shell->buffer[shell->buf_i++] = shell->input[shell->i++];
	while (shell->input[shell->i] && shell->input[shell->i] != quote)
	{
		realloc_buffer(shell);
		shell->buffer[shell->buf_i++] = shell->input[shell->i++];
	}
	if (shell->input[shell->i] == quote)
	{
		realloc_buffer(shell);
		shell->buffer[shell->buf_i++] = shell->input[shell->i++];
	}
}
