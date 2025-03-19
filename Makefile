NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDE = -I ./libft
SRCS = 	src/main.c \
		src/utils.c \
		src/utils1.c \
		src/builtin/echo.c \
		src/builtin/pwd.c \
		src/builtin/exit.c \
		src/builtin/env.c \
		src/builtin/cd.c \
		src/builtin/update_pwd.c \
		src/builtin/export.c \
		src/builtin/export1.c \
		src/builtin/export2.c \
		src/builtin/quicksort.c \
		src/builtin/unset.c \
		src/tokens/tokens.c \
		src/tokens/tokens2.c \
		src/tokens/tokens3.c \
		src/parsing/parsing.c \
		src/parsing/parsing_utils.c \
		src/parsing/parsing_utils1.c \
		src/parsing/parsing_utils2.c \
		src/parsing/parsing_utils3.c \
		src/parsing/parsing_utils4.c \
		src/parsing/parsing_utils5.c \
		src/parsing/parsing_utils6.c \
		src/parsing/parsing_utils7.c \
		src/error_cleanup/error_cleanup.c \
		src/file_helpers.c \
		src/execution/check_command.c \
		src/execution/check_command1.c \
		src/execution/check_command2.c \
		src/execution/check_command3.c \
		src/execution/check_command4.c \
		src/signals/signals.c \
		src/signals/signal_handlers.c
OBJ_DIR = objects
OBJS = $(addprefix $(OBJ_DIR)/,  $(SRCS:.c=.o))
LIBFT = ./libft/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) -o $(NAME) $(OBJS) $(LIBFT) -lreadline

$(LIBFT):
	@$(MAKE) -C ./libft

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	@$(MAKE) clean -C ./libft

fclean: clean
	rm -f $(NAME) $(LIBFT)

re: fclean all

.PHONY: clean fclean re all