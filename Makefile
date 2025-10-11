NAME = minishell

LIBFT = libft/libft.a

SRCS_DIR = src
OBJS_DIR = obj

SRCS = $(SRCS_DIR)/main.c \
       $(SRCS_DIR)/env/env.c

OBJS = $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)

CC = cc

CFLAGS = -Wall -Wextra -Werror -I include -I libft

${NAME}: $(LIBFT) $(OBJS)
	$(CC) $(OBJS) -L libft -lft -o $(NAME)

$(LIBFT):
	make -s -C libft

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

clean:
	rm -f $(OBJS)
	rm -rf $(OBJS_DIR)
	make clean -s -C libft

fclean: clean
	rm -f $(NAME)
	make fclean -s -C libft

re: fclean all

.PHONY: all clean fclean re