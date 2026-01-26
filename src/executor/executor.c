/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 16:34:42 by danslav1e         #+#    #+#             */
/*   Updated: 2026/01/26 01:52:06 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief
 * Checks if a command node represents a built-in command.
 *
 * @param node Command node to check.
 * @return true if built-in, false otherwise.
 */
bool	is_built_in(t_node *node)
{
	if (!node->argv || !node->argv[0])
		return (false);
	if (ft_strncmp(node->argv[0], "echo", 5) == 0)
		return (true);
	if (ft_strncmp(node->argv[0], "cd", 3) == 0)
		return (true);
	if (ft_strncmp(node->argv[0], "pwd", 4) == 0)
		return (true);
	if (ft_strncmp(node->argv[0], "export", 7) == 0)
		return (true);
	if (ft_strncmp(node->argv[0], "unset", 6) == 0)
		return (true);
	if (ft_strncmp(node->argv[0], "env", 4) == 0)
		return (true);
	if (ft_strncmp(node->argv[0], "exit", 5) == 0)
		return (true);
	return (false);
}

/**
 * @brief
 * Executes a built-in command with redirect handling.
 * Saves and restores stdio file descriptors around execution.
 *
 * @param node Command node containing the built-in.
 * @param state Shell state.
 */
void	execute_built_in(t_node *node, t_shell_state *state)
{
	state->saved_fd[0] = dup(STDOUT_FILENO);
	state->saved_fd[1] = dup(STDIN_FILENO);
	if (state->saved_fd[0] == -1 || state->saved_fd[1] == -1)
	{
		if (state->saved_fd[0] != -1)
			close(state->saved_fd[0]);
		if (state->saved_fd[1] != -1)
			close(state->saved_fd[1]);
		state->last_exit_code = FAILURE;
		error_msg("dup", NULL, strerror(errno), FAILURE);
		return ;
	}
	if (apply_redirects(node) == FAILURE)
	{
		state->last_exit_code = FAILURE;
		restore_stdio(state->saved_fd[1], state->saved_fd[0]);
		return ;
	}
	start_built_in(node, state);
	restore_stdio(state->saved_fd[1], state->saved_fd[0]);
}

/**
 * @brief
 * Child process handler for external commands.
 * Applies redirects and executes the binary.
 */
static void	external_child(t_node *node, t_shell_state *state)
{
	if (apply_redirects(node) == FAILURE)
	{
		state->last_exit_code = FAILURE;
		exit_minishell(state);
	}
	execute_bin(node, state);
}

/**
 * @brief
 * Forks and executes an external command.
 * Parent waits for child and captures exit status.
 *
 * @param node Command node to execute.
 * @param state Shell state.
 */
void	execute_external(t_node *node, t_shell_state *state)
{
	pid_t	pid;
	int		status;

	setup_signals_exec();
	pid = fork();
	if (pid == -1)
	{
		state->last_exit_code = FAILURE;
		error_msg("fork", NULL, strerror(errno), FAILURE);
		return ;
	}
	if (pid == 0)
		external_child(node, state);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		state->last_exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		handle_child_signal(status, state);
}

/**
 * @brief
 * Recursively executes an AST node based on its type.
 * Dispatches to appropriate handler for each node type.
 *
 * @param node AST node to execute.
 * @param state Shell state.
 */
void	execute_ast(t_node *node, t_shell_state *state)
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
