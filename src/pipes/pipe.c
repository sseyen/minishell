/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 17:22:44 by danslav1e         #+#    #+#             */
/*   Updated: 2025/11/20 00:41:44 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief
 * Configures and executes the left command in a pipeline.
 * Connects STDOUT to the write end of the pipe.
 */
void	create_left_child(t_shell_state *state, t_node *node, int fd[2])
{
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	close(fd[0]);
	execute_ast(node->left, state);
	free_all_resources(state);
	exit(state->last_exit_code);
}

/**
 * @brief
 * Configures and executes the right command in a pipeline.
 * Connects STDIN to the read end of the pipe.
 */
void	create_right_child(t_shell_state *state, t_node *node, int fd[2])
{
	dup2(fd[0], STDIN_FILENO);
	close(fd[1]);
	close(fd[0]);
	execute_ast(node->right, state);
	free_all_resources(state);
	exit(state->last_exit_code);
}

/**
 * @brief
 * Executes a pipe node (cmd1 | cmd2).
 * Creates a pipe, forks two children, and connects them.
 * The parent waits for both and returns the status of the right child.
 */
void	execute_pipe(t_node *node, t_shell_state *state)
{
	int		fd[2];
	pid_t	left;
	pid_t	right;
	int		s_right;
	int		s_left;

	if (pipe(fd) == -1)
	{
		error_msg("pipe", NULL, strerror(errno), FAILURE);
		state->last_exit_code = FAILURE;
		return ;
	}
	left = fork();
	if (left == -1)
		return (handle_fork_error(state, fd));
	if (left == 0)
		create_left_child(state, node, fd);
	right = fork();
	if (right == -1)
		return (kill_first_process(state, fd, left));
	if (right == 0)
		create_right_child(state, node, fd);
	wait_child_processes(state, left, right, fd);
}
