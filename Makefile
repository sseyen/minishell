NAME = minishell

LIBFT = libft/libft.a

SRCS_DIR = src
OBJS_DIR = obj

SRCS = $(SRCS_DIR)/main.c \
       $(SRCS_DIR)/env/env.c

OBJS = $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)

CC = cc

CFLAGS = -Wall -Wextra -Werror -I include -I libft

# MAIN BUILD

${NAME}: $(LIBFT) $(OBJS)
	$(CC) $(OBJS) -L libft -lft -o $(NAME)

$(LIBFT):
	make -s -C libft

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

# TESTS

TEST = test

TEST_DIR = tests
TEST_OBJS_DIR = obj/tests

TEST_SRCS = $(TEST_DIR)/main_test.c \
			$(TEST_DIR)/env/env_test.c \
			$(TEST_DIR)/lexer/count_tokens_test.c \
			$(TEST_DIR)/lexer/fill_tokens_test.c \
			$(SRCS_DIR)/env/env.c \
			$(SRCS_DIR)/lexer/lexer.c \
			${SRCS_DIR}/lexer/token.c \
			${SRCS_DIR}/lexer/token_utils.c \
			${SRCS_DIR}/lexer/parse.c \
			${SRCS_DIR}/utils/is_operator_char.c \
			${SRCS_DIR}/utils/is_whitespace.c \
			$(SRCS_DIR)/utils/ft_strndup.c

TEST_OBJS = $(TEST_SRCS:%.c=$(TEST_OBJS_DIR)/%.o)

TEST_CFLAGS = $(CFLAGS) -I tests/include -I src/lexer

$(TEST): $(LIBFT) $(TEST_OBJS)
	$(CC) $(TEST_OBJS) -L libft -lft -o $(TEST)

$(TEST_OBJS_DIR)/%.o: %.c
	@mkdir -p $(dir $@) 
	$(CC) $(TEST_CFLAGS) -c $< -o $@

clean_test:
	rm -f $(TEST_OBJS)
	rm -rf $(TEST_OBJS_DIR)
	rm -f $(TEST)

# CLEAN / REBUILD

clean:
	rm -f $(OBJS)
	rm -rf $(OBJS_DIR)
	make clean -s -C libft

fclean: clean clean_test
	rm -f $(NAME)
	make fclean -s -C libft

re: fclean all

.PHONY: all test clean fclean re