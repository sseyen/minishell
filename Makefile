# Executable
NAME        = minishell

# Compiler and flags
CC          = cc
CFLAGS      = -Wall -Wextra -Werror -g  -I include

# Directories
SRC_DIR     = src
TEST_DIR    = tests/builtins
INC_DIR     = include
LIBFT_DIR   = libft
OBJ_DIR     = obj

# Libraries
LIBFT       = $(LIBFT_DIR)/libft.a
LIBS        = -L$(LIBFT_DIR) -lft -lreadline

# Includes
INCLUDES    = -I $(INC_DIR) -I $(LIBFT_DIR)

MAIN        = src/main.c

# Main object file
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

# Object files
OBJS        = $(SRCS:%.c=$(OBJ_DIR)/%.o)

# Rules
all: $(NAME)

# Target to build the executable
$(NAME): $(LIBFT) $(OBJS) $(MAIN_OBJ)
	@echo "Compiling $(NAME)..."
	$(CC) $(CFLAGS) $(MAIN_OBJ) $(OBJS) $(LIBS) -o $(NAME)
	@echo "$(NAME) ready!"

# Libft compilation
$(LIBFT):
	@echo "Compiling libft..."
	@make -s -C $(LIBFT_DIR)

# Compilation of .c to obj/%.o
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

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
