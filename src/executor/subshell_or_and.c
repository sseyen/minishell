/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_or_and.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 18:35:53 by danslav1e         #+#    #+#             */
/*   Updated: 2025/11/11 19:40:26 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** Выполняет узел SUBSHELL.
** Это всегда делается в дочернем процессе (fork),
** чтобы изолировать изменения (например, cd или export)
** от родительской оболочки.
*/
void	execute_subshell(t_node *node, t_shell_state *state)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		state->last_exit_code = FAILURE;
		return ;
	}
	if (pid == 0)
	{
		// Мы - дочерний процесс (subshell)

		// 1. ПРИМЕНЯЕМ РЕДИРЕКТЫ, ПРИКРЕПЛЕННЫЕ К SUBSHELL
		// Это изменит STDOUT/STDIN *только* для этого дочернего процесса.
		if (apply_redirects(node) == FAILURE)
			exit(FAILURE);

		// 2. Рекурсивно выполняем все, что внутри скобок
		// (ls, echo, cat теперь будут писать в 'output.txt')
		execute_ast(node->child, state);

		// 3. Умираем и передаем наш *конечный* last_exit_code родителю
		exit(state->last_exit_code);
	}
	// Мы - родительский процесс
	// Ждем, пока subshell завершится
	waitpid(pid, &status, 0);

	// Обновляем $? родителя на основе $? subshell-а
	if (WIFEXITED(status))
		state->last_exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		state->last_exit_code = 128 + WTERMSIG(status);
}

/**
 * @brief Выполняет узел 'A && B'.
 *
 * 1. Выполняет 'A' (node->left).
 * 2. Проверяет код возврата 'A' (state->last_exit_code).
 * 3. Если 'A' завершился успешно (код 0), выполняет 'B' (node->right).
 */
void    execute_and(t_node *node, t_shell_state *state)
{
	// 1. Выполняем 'A' (левый узел)
	execute_ast(node->left, state);
	
	// 2. Проверяем код возврата 'A'.
	// 3. Если 'A' == SUCCESS (0), выполняем 'B' (правый узел).
	if (state->last_exit_code == SUCCESS)
	{
		execute_ast(node->right, state);
	}
}

/**
 * @brief Выполняет узел 'A || B'.
 *
 * 1. Выполняет 'A' (node->left).
 * 2. Проверяет код возврата 'A' (state->last_exit_code).
 * 3. Если 'A' завершился с ошибкой (код != 0), выполняет 'B' (node->right).
 */
void    execute_or(t_node *node, t_shell_state *state)
{
	// 1. Выполняем 'A' (левый узел)
	execute_ast(node->left, state);

	// 2. Проверяем код возврата 'A'.
	// 3. Если 'A' == FAILURE (не 0), выполняем 'B' (правый узел).
	if (state->last_exit_code != SUCCESS)
	{
		execute_ast(node->right, state);
	}
}
