/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 16:41:12 by danslav1e         #+#    #+#             */
/*   Updated: 2025/11/11 22:12:09 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** Проверяет, не вызовет ли добавление следующей цифры (digit)
** переполнение `long long`, зная текущее число (n) и знак (sign).
*/
static int	check_overflow(unsigned long long n, char c_digit, int sign)
{
	unsigned long long	cutoff;
	unsigned long long	cutlim;

	// 'cutoff' - это LLONG_MAX / 10 (все цифры, кроме последней)
	// 'cutlim' - это LLONG_MAX % 10 (последняя цифра)
	if (sign == 1)
	{
		// Для положительных: 9223372036854775807
		cutoff = 922337203685477580;
		cutlim = 7;
	}
	else
	{
		// Для отрицательных: -9223372036854775808
		// (как unsigned long long, это 9223372036854775808)
		cutoff = 922337203685477580;
		cutlim = 8;
	}
	// Проверка:
	// 1. (n > cutoff): Мы уже вышли за пределы (e.g. 922337203685477581)
	// 2. (n == cutoff && (c_digit - '0') > cutlim):
	//    Мы на грани, и следующая цифра слишком большая (e.g. 9223372036854775808)
	if (n > cutoff || (n == cutoff && (c_digit - '0') > cutlim))
		return (1); // Переполнение
	return (0);
}

/*
** Проверяет, является ли строка 'str' валидным 'long long'.
** Это ловит "exit test" (не-цифры) и "exit 999...999" (переполнение).
*/
static int	is_valid_llong(char *str)
{
	int					i;
	int					sign;
	unsigned long long	n;

	i = 0;
	sign = 1;
	n = 0;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (str[i] == '\0')
		return (0); // Строка была "+\0" или "-\0"
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0); // Не-цифра (e.g. "123a")
		if (check_overflow(n, str[i], sign))
			return (0); // Переполнение (e.g. "999...999")
		n = n * 10 + (str[i] - '0');
		i++;
	}
	return (1);
}

/*
** TODO: Ты должен создать эту функцию.
** Она должна освободить *всю* память, выделенную для
** (1) AST (t_node), (2) envp, (3) и т.д.
*/
static void	free_all_resources(t_shell_state *state)
{
	(void)state; // Убрать, когда реализуешь
	// 1. Освободить копию envp
	//    (Например: free_split_array(state->envp);)
	// 2. Освободить все дерево AST
	//    (Например: free_ast_tree(state->first_node);)
	// 3. Очистить историю readline (необязательно, но чисто)
	//    rl_clear_history();
}

/*
** Завершает работу minishell с указанным кодом или последним кодом ошибки.
** Соответствует поведению bash.
*/
int	built_in_exit(t_node *node, t_shell_state *state)
{
	unsigned char	exit_code;

	if (node->argc == 1)
	{
		// Сценарий 1: "exit" (без аргументов)
		ft_putendl_fd("exit", STDOUT_FILENO); // Печатаем, т.к. выходим
		free_all_resources(state); // Очистка
		exit(state->last_exit_code);
	}
	if (!is_valid_llong(node->argv[1]))
	{
		// Сценарий 3: "exit test" ИЛИ "exit [overflow]"
		// НЕ печатаем "exit"
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(node->argv[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		state->last_exit_code = 2;
		return (FAILURE); // НЕ ЗАВЕРШАЕМ
	}
	if (node->argc > 2)
	{
		// Сценарий 4: "exit 1 2"
		// НЕ печатаем "exit"
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		state->last_exit_code = 1;
		return (FAILURE); // НЕ ЗАВЕРШАЕМ
	}
	// Сценарий 2: "exit [n]" (n - валидное число)
	ft_putendl_fd("exit", STDOUT_FILENO); // Печатаем, т.к. выходим
	exit_code = (unsigned char)ft_atoi(node->argv[1]);
	//free_all(state); // Очистка
	exit(exit_code);
}
