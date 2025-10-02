NAME = minishell

LIBFT = libft/libft.a

SRCS = src/main.c

OBJS = $(SRCS:.c=.o)

CC = cc

CFLAGS = -Wall -Wextra -Werror

$(LIBFT):
	make -s -C libft

.o: .c
	$(CC) $(CFLAGS) -c $< -o $@

${NAME}: $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) \
		-I libft -I include -L libft -lft

all: $(NAME)

clean:
	rm -f $(OBJS)
	make clean -s -C libft

fclean: clean
	rm -f $(NAME)
	make fclean -s -C libft

re: fclean all

.PHONY: all clean fclean re