/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 20:09:45 by smishos           #+#    #+#             */
/*   Updated: 2025/03/19 20:09:46 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	realloc_buffer(t_ms *shell)
{
	shell->new_buffer = ft_realloc(shell->buffer, shell->buf_count, \
		shell->buf_count + 1);
	if (!shell->new_buffer)
		malloc_error(shell);
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
