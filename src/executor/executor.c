/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 16:34:42 by danslav1e         #+#    #+#             */
/*   Updated: 2025/11/11 16:36:49 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// check if command is built_in or not
bool is_built_in(t_node *node)
{
    if (ft_strncmp(node->argv[0], "echo", 5) == 0)
        return (true);
    else if (ft_strncmp(node->argv[0], "cd", 3) == 0)
        return (true);
    else if (ft_strncmp(node->argv[0], "pwd", 4) == 0)
        return (true);
    else if (ft_strncmp(node->argv[0], "export", 7) == 0)
        return (true);
    else if (ft_strncmp(node->argv[0], "unset", 6) == 0)
        return (true);
    else if (ft_strncmp(node->argv[0], "env", 4) == 0)
        return (true);
    else if (ft_strncmp(node->argv[0], "exit", 5) == 0)
        return (true);
    return (false);
}

/*
** Логика "А": Выполнить built-in (в родительском процессе).
** Мы ОБЯЗАНЫ сохранить и восстановить STDIN/STDOUT.
*/
void	execute_built_in(t_node *node, t_shell_state *state)
{
	int	saved_stdout;
	int	saved_stdin;

	// 1. Сохраняем оригинальные STDIN/STDOUT
	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdout == -1 || saved_stdin == -1)
	{
		perror("dup");
		state->last_exit_code = FAILURE;
		return ;
	}
	
	// 2. Применяем редиректы (которые вызовут dup2)
	if (apply_redirects(node) == FAILURE)
	{
		state->last_exit_code = FAILURE;
		restore_stdio(saved_stdin, saved_stdout);
		return ;
	}
	
	// 3. Выполняем built-in (он будет писать/читать в/из файлов)
	// (Это твой код из execute_built_in)
	if (ft_strncmp(node->argv[0], "echo", 5) == 0)
		state->last_exit_code = built_in_echo(node);
	else if (ft_strncmp(node->argv[0], "cd", 3) == 0)
		state->last_exit_code = built_in_cd(node, state); // (cd должен принимать state)
	else if (ft_strncmp(node->argv[0], "pwd", 4) == 0)
		state->last_exit_code = built_in_pwd(node); // (pwd должен принимать node)
	else if (ft_strncmp(node->argv[0], "export", 7) == 0)
		state->last_exit_code = built_in_export(node, state);
	else if (ft_strncmp(node->argv[0], "unset", 6) == 0)
		state->last_exit_code = built_in_unset(node, state);
	else if (ft_strncmp(node->argv[0], "env", 4) == 0)
		state->last_exit_code = built_in_env(node, state);
	else if (ft_strncmp(node->argv[0], "exit", 5) == 0)
		state->last_exit_code = built_in_exit(node, state);
	
	// 4. ВОССТАНАВЛИВАЕМ STDIN И STDOUT (критически важно!)
	restore_stdio(saved_stdin, saved_stdout);
}

/*
** Логика "Б": Выполнить внешнюю команду (в дочернем процессе).
** Мы НЕ сохраняем STDIN/STDOUT, т.к. мы в ребенке.
*/
void	execute_external(t_node *node, t_shell_state *state)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		state->last_exit_code = FAILURE;
		return ;
	}
	if (pid == 0)
	{
		// Мы - ребенок
		// 1. Применяем редиректы
		if (apply_redirects(node) == FAILURE)
			exit(FAILURE); // Выходим с ошибкой 1

		// 2. Выполняем команду
		execute_bin(node, state); // (Эта функция должна вызывать execve)
		
		// 3. (Если execve провалился, execute_bin должна выйти сама)
		//    На всякий случай:
		exit(state->last_exit_code);
	}
	
	// Мы - родитель
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		state->last_exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		state->last_exit_code = 128 + WTERMSIG(status);
}

//main executing function, going throught all the nodes of AST with recursion
void execute_ast(t_node *node, t_shell_state *state)
{
    if (!node)
        return ;
    if (node->type == NODE_CMD)
    {
        if (is_built_in(node))
            execute_built_in(node, state);
        else
            execute_external(node, state);
    }
    else if (node->type == NODE_PIPE)
        execute_pipe(node, state);
    else if (node->type == NODE_SUBSHELL)
        execute_subshell(node, state);
    else if (node->type == NODE_OR)
        execute_or(node, state);
    else if (node->type == NODE_AND)
        execute_and(node, state);
}
