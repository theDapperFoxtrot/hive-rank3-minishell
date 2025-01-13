#ifndef MINISHELL_H
# define MINISHELL_H
# include "../libft/libft.h"
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

// REMOVE
# include <stdio.h>

// Simplified token types
enum token_type {
	TOKEN_ARGS,       // ARGS, including quoted strings
	TOKEN_PIPE,      // |
	TOKEN_REDIR_IN,  // <
	TOKEN_HERE_DOC,  // <<
	TOKEN_REDIR_OUT, // >
	TOKEN_APPEND     // >>
};

typedef struct s_token {
	char *value;
	int type;
	struct s_token *next;
} t_token;

typedef struct s_ms
{
	int		exit_code;
	char	**env_list;
	char 	*input;
	t_token *token;
} 	t_ms;


//utils.c
void	free_args(char **commands);
void	free_env(t_ms *shell);
void	print_error(char *message, t_ms *shell, int status);
int		count_args(char **command);
//builtin
void	check_command(char **command, t_ms *shell);
void	ft_echo(char **command, t_ms *shell);
void	ft_pwd(char **command, t_ms *shell);
void	ft_exit(char **command, t_ms *shell);
void	ft_env(char **command, t_ms *shell);
void	ft_cd(char **command, t_ms *shell);
void	create_env(t_ms *shell, char **envp);
int		update_pwd(t_ms *shell, char *string, char *value);
int		env_list_size(char **envp);
// parser
void	tokenize_input(t_ms *shell);
void print_tokens(t_token *tokens);


#endif