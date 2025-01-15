NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -g
INCLUDE = -I ./libft
SRCS = 	src/main.c \
		src/utils.c \
		src/builtin/echo.c \
		src/builtin/check_command.c \
		src/builtin/pwd.c \
		src/builtin/exit.c \
		src/builtin/env.c \
		src/builtin/cd.c \
		src/builtin/update_pwd.c \
		src/tokens/tokens.c
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