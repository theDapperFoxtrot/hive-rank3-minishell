#include "../../include/minishell.h"

char	*parse_quotes(char *str)
{
	int		i;
	int		j;
	char	*parsed;

	i = 0;
	j = 0;
	if (!str)
		return (NULL);
	while (str[i])
	{
		if (str[i] != 34 || str[i] != 39)
			j++;
		i++;
	}
	parsed = malloc((j + 1) * sizeof(char));
	if (parsed == NULL)
		return (NULL);
	j = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] != 34 && str[i] != 39)
			parsed[j++] = str[i];
		i++;
	}
	parsed[j] = '\0';
	return (parsed);
}

void	setup_delim(t_ms *shell, t_command *cmd, t_token *token)
{
	char	*stripped;

	stripped = parse_quotes(token->value);
	cmd->heredoc_delimiter = ft_strdup(stripped);
	free(stripped);
	if (!cmd->heredoc_delimiter)
		malloc_error(shell);
}

void	allocate_heredoc_lines(t_ms *shell, t_command *cmd)
{
	cmd->heredoc_lines = (char **) malloc(sizeof(char *) * 2);
	if (!cmd->heredoc_lines)
		malloc_error(shell);
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
		check_signals(SIGINT, &sig_handler_heredoc);
		check_signals(SIGQUIT, &sig_handler_heredoc);
		cmd->heredoc_lines = ft_realloc(cmd->heredoc_lines, sizeof(char *) * i, sizeof(char *) * (i + 1));
		if (!cmd->heredoc_lines)
			malloc_error(shell);
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


