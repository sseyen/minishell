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
SRCS_TEST   = $(TEST_DIR)/test_pwd.c

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

# Очистка
clean:
	@echo "Cleaning objects..."
	@rm -f $(OBJS)
	@make -s -C $(LIBFT_DIR) clean

fclean: clean
	@echo "Cleaning executable..."
	@rm -f $(NAME)
	@make -s -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re