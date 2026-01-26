# Имя исполнимого файла для теста
NAME        = minishell

# Компилятор и флаги
CC          = cc
CFLAGS      = -Wall -Wextra -Werror -g  -I include

# Директории
SRC_DIR     = src
TEST_DIR    = tests/builtins
INC_DIR     = include
LIBFT_DIR   = libft
OBJ_DIR     = obj

# Библиотеки
LIBFT       = $(LIBFT_DIR)/libft.a
LIBS        = -L$(LIBFT_DIR) -lft -lreadline

# Включаемые файлы (Header files)
INCLUDES    = -I $(INC_DIR) -I $(LIBFT_DIR)

MAIN        = src/main.c

# MAIN object (built separately so it won't be included in tests)
MAIN_OBJ    = $(OBJ_DIR)/$(MAIN:.c=.o)

# Source files
SRCS        = \
              src/execute_bin/execute_bin.c \
              src/execute_bin/execute_bin_utils.c \
              src/executor/executor.c \
              src/executor/executor_utils.c \
              src/executor/subshell_or_and.c \
              src/parse/build_ast.c \
              src/parse/handle_line.c \
              src/parse/parse_expr.c \
              src/parse/parse_expr_utils.c \
              src/parse/parse_command.c \
              src/parse/parser_utils.c \
              src/parse/parser_redirect_utils.c \
              src/redirects/heredoc.c \
              src/redirects/heredoc_utils.c \
              src/redirects/redirects.c \
              src/pipes/pipe.c \
              src/pipes/pipe_utils.c \
              src/env/env.c \
              src/env/env_utils.c \
              src/builtins/built_in_cd.c \
              src/builtins/built_in_echo.c \
              src/builtins/built_in_env.c \
              src/builtins/built_in_exit.c \
              src/builtins/built_in_export.c \
              src/builtins/built_in_export_utils.c \
              src/builtins/built_in_pwd.c \
              src/builtins/built_in_unset.c \
              src/utils/errors.c \
              src/utils/syntax_error.c \
              src/utils/free_ast.c \
              src/utils/is_whitespace.c \
              src/utils/is_operator_char.c \
              src/utils/ft_strndup.c \
              src/utils/ft_strnjoin.c \
              src/signals/signals.c \
              src/signals/signals_utils.c \
              src/lexer/lexer.c \
              src/lexer/parse.c \
              src/lexer/token.c \
              src/lexer/token_utils.c \
              src/lexer/validate.c \
              src/lexer/validate_utils.c \
              src/lexer/expand.c \
              src/lexer/expand_utils.c

# Объектные файлы в отдельной папке `obj/`, сохраняя относительные пути
OBJS        = $(SRCS:%.c=$(OBJ_DIR)/%.o)

# Правила
all: $(NAME)

# Компиляция основной программы
$(NAME): $(LIBFT) $(OBJS) $(MAIN_OBJ)
	@echo "Compiling $(NAME)..."
	$(CC) $(CFLAGS) $(MAIN_OBJ) $(OBJS) $(LIBS) -o $(NAME)
	@echo "$(NAME) ready!"

# Компиляция libft
$(LIBFT):
	@echo "Compiling libft..."
	@make -s -C $(LIBFT_DIR)

# Компиляция .c в obj/%.o (создаёт директории при необходимости)
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# TESTS

TEST = test

TEST_DIR = tests
TEST_OBJS_DIR = obj/tests



TEST_SRCS = $(TEST_DIR)/main_test.c \
            $(TEST_DIR)/builtins/pwd_test.c \
            $(TEST_DIR)/utils/print_test_header.c \
            $(TEST_DIR)/utils/print_result.c \
            $(TEST_DIR)/utils/print_summary.c \
            $(TEST_DIR)/utils/print_tokens.c \
			$(TEST_DIR)/env/env_test.c \
			$(TEST_DIR)/lexer/count_tokens_test.c \
			$(TEST_DIR)/lexer/fill_tokens_test.c \
            $(TEST_DIR)/lexer/expand_tokens_test.c \
            $(TEST_DIR)/ast/build_ast_test.c \
            $(TEST_DIR)/ast/print_ast.c

TEST_OBJS = $(TEST_SRCS:%.c=$(TEST_OBJS_DIR)/%.o)

TEST_CFLAGS = $(CFLAGS) -I tests/include -I src/lexer

$(TEST): $(LIBFT) $(TEST_OBJS) $(OBJS)
	$(CC) $(TEST_OBJS) $(OBJS) $(LIBS) -o $(TEST)

$(TEST_OBJS_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(TEST_CFLAGS) -c $< -o $@

clean_test:
	rm -f $(TEST_OBJS)
	rm -rf $(TEST_OBJS_DIR)
	rm -f $(TEST)

# CLEAN / REBUILD

clean:
	@echo "Cleaning objects..."
	@rm -f $(OBJS)
	@rm -rf $(OBJ_DIR)
	@make -s -C $(LIBFT_DIR) clean

fclean: clean
	@echo "Cleaning executable..."
	@rm -f $(NAME) $(TEST)
	@make -s -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all test clean fclean re
