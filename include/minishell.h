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

typedef struct s_command {
	char	**args;
	struct	s_command *next;
	char	*input_file;
	char	*output_file;
	char	*heredoc_delimiter;
	int		append_mode;
} t_command;


typedef struct s_token {
	char	*value;
	int		type;
	struct	s_token *next;
} t_token;

typedef struct s_ms
{
	unsigned char	exit_code;
	char			**env_list;
	char 			*input;
	int				pipe_count;
	t_token 		*token;
	t_command 		*commands;
	int				i;
	char 			buffer[100000];
	int				buf_i;
	int				type;
} 	t_ms;


//utils.c
void	free_args(char **commands);
void	free_env(t_ms *shell);
void	print_error(char *message, t_ms *shell, unsigned char status);
int		count_args(char **command);
//builtin
void	check_command(t_ms *shell);
void	ft_echo(char **command, t_ms *shell);
void	ft_pwd(char **command, t_ms *shell);
void	ft_exit(char **command, t_ms *shell);
void	ft_env(char **command, t_ms *shell);
void	ft_cd(char **command, t_ms *shell);
void	create_env(t_ms *shell, char **envp);
int		update_pwd(t_ms *shell, char *string, char *value);
int		env_list_size(char **envp);
void	ft_export(char **command, t_ms *shell);
void	quicksort(char **arr, int low, int high);
void	ft_unset(char **command, t_ms *shell);
// parser
// void	print_tokens(t_token *tokens);
// tokens folder
void	tokenize_input(t_ms *shell);
int		is_operator(char c);
void	write_token_args(t_ms *shell);
void	if_is_operator(t_ms *shell);
void	create_token(t_ms *shell);
void	add_token(t_ms *shell, t_token *new_token);
// errors and cleanup
void	cleanup(t_ms *shell);
void	free_commands(t_command *commands);
void	free_tokens(t_ms *shell);




#endif