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

void	parse_tokens(t_ms *shell)
{
	t_token		*token;
	t_command	*cmd;
	t_command	*prev_cmd;

	prev_cmd = NULL;
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return ;
	ft_bzero(cmd, sizeof(t_command));
	cmd->args = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->append_mode = 0;
	cmd->next = NULL;
	shell->commands = cmd;
	token = shell->token;
	while (token)
	{
		if (token->type == TOKEN_ARGS)
			add_argument(cmd, token->value);
		else if (token->type == TOKEN_PIPE)
		{
			prev_cmd = cmd;
			cmd = malloc(sizeof(t_command));
			if (!cmd)
				return ;
			ft_bzero(cmd, sizeof(t_command));
			cmd->args = NULL;
			cmd->input_file = NULL;
			cmd->output_file = NULL;
			cmd->append_mode = 0;
			cmd->next = NULL;
			prev_cmd->next = cmd;
		}
		else if (token->type == TOKEN_REDIR_IN)
		{
			token = token->next;
			if (token && token->type == TOKEN_ARGS)
				cmd->input_file = ft_strdup(token->value);
		}
		else if (token->type == TOKEN_REDIR_OUT)
		{
			token = token->next;
			if (token && token->type == TOKEN_ARGS)
				cmd->output_file = ft_strdup(token->value);
		}
		token = token->next;
	}
}
