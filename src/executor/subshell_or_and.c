/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_or_and.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 18:35:53 by danslav1e         #+#    #+#             */
/*   Updated: 2026/01/26 01:52:06 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief
 * Child process handler for subshell execution.
 * Sets up signals, applies redirects, and executes child AST.
 */
static void	subshell_child(t_node *node, t_shell_state *state)
{
	setup_signals_child();
	state->is_child = 1;
	if (apply_redirects(node) == FAILURE)
	{
		state->last_exit_code = FAILURE;
		exit_minishell(state);
	}
	execute_ast(node->child, state);
	exit_minishell(state);
}

/**
 * @brief
 * Executes a subshell node in a forked child process.
 * Parent waits for child and captures exit status.
 *
 * @param node Subshell node.
 * @param state Shell state.
 */
void	execute_subshell(t_node *node, t_shell_state *state)
{
	pid_t	pid;
	int		status;

	setup_signals_exec();
	pid = fork();
	if (pid == -1)
	{
		error_msg("fork", NULL, strerror(errno), FAILURE);
		state->last_exit_code = FAILURE;
		return ;
	}
	if (pid == 0)
		subshell_child(node, state);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		state->last_exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		handle_child_signal(status, state);
}

/**
 * @brief
 * Executes an 'AND' logic node (cmd1 && cmd2).
 *
 * Executes the left child. If it succeeds (exit code 0),
 * executes the right child.
 */
void	execute_and(t_node *node, t_shell_state *state)
{
	execute_ast(node->left, state);
	if (state->last_exit_code == SUCCESS)
	{
		execute_ast(node->right, state);
	}
}

/**
 * @brief
 * Executes an 'OR' logic node (cmd1 || cmd2).
 *
 * Executes the left child. If it fails (exit code != 0),
 * executes the right child.
 */
void	execute_or(t_node *node, t_shell_state *state)
{
	execute_ast(node->left, state);
	if (state->last_exit_code != SUCCESS && state->last_exit_code < 128)
	{
		execute_ast(node->right, state);
	}
}
