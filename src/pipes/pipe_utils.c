/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 00:39:46 by danslav1e         #+#    #+#             */
/*   Updated: 2026/01/26 01:52:06 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief
 * Handles fork failure during pipe execution.
 * Prints error, closes pipe file descriptors.
 */
void	handle_fork_error(t_shell_state *state, int fd[2])
{
	error_msg("fork", NULL, strerror(errno), FAILURE);
	state->last_exit_code = FAILURE;
	close(fd[0]);
	close(fd[1]);
}

/**
 * @brief
 * Waits for both pipe child processes to complete.
 * Captures exit status from right side (last command in pipe).
 *
 * @param state Shell state to update exit code.
 * @param left PID of left child.
 * @param right PID of right child.
 * @param fd Pipe file descriptors to close.
 */
void	wait_child_processes(t_shell_state *state, pid_t left, pid_t right,
		int fd[2])
{
	int	s_left;
	int	s_right;
	int	sig;

	close(fd[0]);
	close(fd[1]);
	waitpid(left, &s_left, 0);
	waitpid(right, &s_right, 0);
	if (WIFEXITED(s_right))
		state->last_exit_code = WEXITSTATUS(s_right);
	else if (WIFSIGNALED(s_right))
	{
		sig = WTERMSIG(s_right);
		state->last_exit_code = 128 + sig;
		if (sig == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		else if (sig == SIGQUIT)
			ft_putendl_fd("Quit: 3", STDOUT_FILENO);
	}
}

/**
 * @brief
 * Kills the first child process when second fork fails.
 * Ensures cleanup of left process before handling error.
 */
void	kill_first_process(t_shell_state *state, int fd[2], pid_t left)
{
	kill(left, SIGTERM);
	waitpid(left, NULL, 0);
	handle_fork_error(state, fd);
}
