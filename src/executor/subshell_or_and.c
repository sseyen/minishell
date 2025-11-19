/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_or_and.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 18:35:53 by danslav1e         #+#    #+#             */
/*   Updated: 2025/11/19 19:42:16 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief
 * Executes a subshell (commands inside parentheses).
 *
 * Creates a child process (fork) to isolate the subshell's environment.
 * Redirects are applied to the subshell process itself.
 * The parent waits for the subshell to finish and updates the exit code.
 *
 * @param node The AST node representing the subshell.
 * @param state The shell state.
 */
void	execute_subshell(t_node *node, t_shell_state *state)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		error_msg("fork", NULL, strerror(errno), FAILURE);
		state->last_exit_code = FAILURE;
		return ;
	}
	if (pid == 0)
	{
		if (apply_redirects(node) == FAILURE)
		{
			free_all_resources(state);
			exit(FAILURE);
		}
		execute_ast(node->child, state);
		exit(free_all_resources(state));
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		state->last_exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		state->last_exit_code = 128 + WTERMSIG(status);
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
	if (state->last_exit_code != SUCCESS)
	{
		execute_ast(node->right, state);
	}
}
