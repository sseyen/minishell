# Имя исполнимого файла для теста
NAME        = test_pwd

# Компилятор и флаги
CC          = cc
CFLAGS      = -Wall -Wextra -Werror -g

# Директории
SRC_DIR     = src
TEST_DIR    = tests/builtins
INC_DIR     = include
LIBFT_DIR   = libft

# Библиотеки
LIBFT       = $(LIBFT_DIR)/libft.a
LIBS        = -L$(LIBFT_DIR) -lft -lreadline

# Включаемые файлы (Header files)
INCLUDES    = -I $(INC_DIR) -I $(LIBFT_DIR)

# Исходные файлы проекта (ИСКЛЮЧАЯ src/main.c)
# Мы используем wildcard для автоматического поиска всех .c файлов в подпапках src
SRCS_PROJECT = $(shell find $(SRC_DIR) -name "*.c" ! -name "main.c")

# Файл теста
SRCS_TEST   = 

# Объединяем все исходники
SRCS        = $(SRCS_PROJECT) $(SRCS_TEST)

# Объектные файлы
OBJS        = $(SRCS:.c=.o)

# Правила
all: $(NAME)

# Компиляция основной программы
$(NAME): $(LIBFT) $(OBJS)
	@echo "Compiling $(NAME)..."
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)
	@echo "$(NAME) ready!"

# Компиляция libft
$(LIBFT):
	@echo "Compiling libft..."
	@make -s -C $(LIBFT_DIR)

# Компиляция .c в .o
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# TESTS

TEST = test

TEST_DIR = tests
TEST_OBJS_DIR = obj/tests

TEST_SRCS = $(TEST_DIR)/main_test.c \
			$(TEST_DIR)/test_pwd.c
			$(TEST_DIR)/utils/print_tokens.c \
			$(TEST_DIR)/env/env_test.c \
			$(TEST_DIR)/lexer/count_tokens_test.c \
			$(TEST_DIR)/lexer/fill_tokens_test.c \
			$(TEST_DIR)/lexer/expand_tokens_test.c \
			$(SRCS_DIR)/env/env.c \
			$(SRCS_DIR)/lexer/lexer.c \
			${SRCS_DIR}/lexer/token.c \
			${SRCS_DIR}/lexer/token_utils.c \
			${SRCS_DIR}/lexer/parse.c \
			$(SRCS_DIR)/lexer/expand.c \
			${SRCS_DIR}/lexer/expand_utils.c \
			${SRCS_DIR}/utils/is_operator_char.c \
			${SRCS_DIR}/utils/is_whitespace.c \
			$(SRCS_DIR)/utils/ft_strndup.c \
			$(SRCS_DIR)/utils/ft_strnjoin.c

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
	make clean -s -C libft

fclean: clean
	@echo "Cleaning executable..."
	@rm -f $(NAME)
	@make -s -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all test clean fclean re