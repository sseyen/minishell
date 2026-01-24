/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 16:34:42 by danslav1e         #+#    #+#             */
/*   Updated: 2026/01/24 18:50:33 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief
 * Checks if a command node represents a built-in command.
 *
 * @param node
 * The AST node to check.
 * @return
 * `true` if argv[0] matches a built-in name (echo, cd, pwd, etc.),
 * `false` otherwise.
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
 * Executes a built-in command in the parent process.
 * Saves STDIN/STDOUT, applies redirects, runs the command,
 * and restores STDIN/STDOUT.
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

void	start_built_in(t_node *node, t_shell_state *state)
{
	if (ft_strncmp(node->argv[0], "echo", 5) == 0)
		state->last_exit_code = built_in_echo(node);
	else if (ft_strncmp(node->argv[0], "cd", 3) == 0)
		state->last_exit_code = built_in_cd(node, state);
	else if (ft_strncmp(node->argv[0], "pwd", 4) == 0)
		state->last_exit_code = built_in_pwd(node);
	else if (ft_strncmp(node->argv[0], "export", 7) == 0)
		state->last_exit_code = built_in_export(node, state);
	else if (ft_strncmp(node->argv[0], "unset", 6) == 0)
		state->last_exit_code = built_in_unset(node, state);
	else if (ft_strncmp(node->argv[0], "env", 4) == 0)
		state->last_exit_code = built_in_env(node, state);
	else if (ft_strncmp(node->argv[0], "exit", 5) == 0)
		state->last_exit_code = built_in_exit(node, state);
}

/**
 * @brief
 * Executes an external command (binary) in a child process.
 * Forks, applies redirects in the child, and execs.
 */
void	execute_external(t_node *node, t_shell_state *state)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		state->last_exit_code = FAILURE;
		error_msg("fork", NULL, strerror(errno), FAILURE);
		return ;
	}
	if (pid == 0)
	{
		if (apply_redirects(node) == FAILURE)
		{
			state->last_exit_code = FAILURE;
			exit_minishell(state);
		}
		execute_bin(node, state);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		state->last_exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		state->last_exit_code = 128 + WTERMSIG(status);
}

/**
 * @brief
 * The main recursive execution dispatcher.
 * Traverses the AST and calls the appropriate execution function
 * based on the node type.
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
