/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 19:17:44 by danslav1e         #+#    #+#             */
/*   Updated: 2026/01/14 19:50:13 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

// Вспомогательная функция для перевода типа ноды в строку
static char	*get_node_type_str(t_node_type type)
{
	if (type == NODE_CMD)
		return ("CMD");
	if (type == NODE_PIPE)
		return ("PIPE (|)");
	if (type == NODE_AND)
		return ("AND (&&)");
	if (type == NODE_OR)
		return ("OR (||)");
	if (type == NODE_SUBSHELL)
		return ("SUBSHELL");
	return ("UNKNOWN");
}

// Печатает содержимое конкретной ноды (аргументы или тип)
static void	print_node_content(t_node *node)
{
	int	i;

	printf("\033[1;36m[%s]\033[0m ", get_node_type_str(node->type));
	
	if (node->type == NODE_CMD && node->argv)
	{
		printf("\033[0;33m"); // Желтый цвет для аргументов
		i = 0;
		while (node->argv[i])
		{
			printf("%s ", node->argv[i]);
			i++;
		}
		printf("\033[0m");
	}
	// Если есть редиректы, можно добавить их вывод здесь
	if (node->redirects)
		printf(" <has redirects>");
	printf("\n");
}

/*
** Рекурсивная функция отрисовки
** prefix: строка с отступами для текущего уровня
** is_left: флаг, является ли нода левым ребенком (для красоты линий)
*/
static void	print_ast_recursive(t_node *node, char *prefix, int is_left)
{
	char	*new_prefix;
	char	*addon;

	if (!node)
		return ;

	// 1. Печатаем саму ноду
	printf("%s", prefix);
	if (is_left)
		printf("├── ");
	else
		printf("└── ");

	print_node_content(node);

	// 2. Подготовка префикса для детей
	if (is_left)
		addon = "│   ";
	else
		addon = "    ";

	// Выделяем память под префикс
	new_prefix = calloc(strlen(prefix) + strlen(addon) + 1, sizeof(char));
	if (!new_prefix)
		return ;
	strcpy(new_prefix, prefix);
	strcat(new_prefix, addon);

	// 3. РЕКУРСИЯ
	
	// СЛУЧАЙ А: Это SUBSHELL, у него есть поле child (проверь, как оно называется у тебя в struct s_node)
	// Обычно оно называется 'child' или 'tree'.
	if (node->type == NODE_SUBSHELL && node->child)
	{
		// Рисуем содержимое скобок как "правого" (последнего) ребенка, чтобы закрыть ветку красиво
		print_ast_recursive(node->child, new_prefix, 0);
	}
	// СЛУЧАЙ Б: Обычные ноды (PIPE, AND, OR)
	else if (node->left || node->right)
	{
		print_ast_recursive(node->left, new_prefix, 1);
		print_ast_recursive(node->right, new_prefix, 0);
	}
	
	free(new_prefix);
}

// Точка входа для вызова из main
void	print_ast(t_node *root)
{
	printf("\n\033[1;37m=== AST VISUALIZATION ===\033[0m\n");
	if (!root)
	{
		printf("(Empty Tree)\n");
		return ;
	}
	// Начальный вызов с пустым префиксом
	print_ast_recursive(root, "", 0);
	printf("\033[1;37m=========================\033[0m\n\n");
}
