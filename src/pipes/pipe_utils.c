/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 00:39:46 by danslav1e         #+#    #+#             */
/*   Updated: 2025/11/20 00:41:13 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_fork_error(t_shell_state *state, int fd[2])
{
	error_msg("fork", NULL, strerror(errno), FAILURE);
	state->last_exit_code = FAILURE;
	close(fd[0]);
	close(fd[1]);
}

void	wait_child_processes(t_shell_state *state, pid_t left, pid_t right,
		int fd[2])
{
	int	s_left;
	int	s_right;

	close(fd[0]);
	close(fd[1]);
	waitpid(left, &s_left, 0);
	waitpid(right, &s_right, 0);
	if (WIFEXITED(s_right))
		state->last_exit_code = WEXITSTATUS(s_right);
	else if (WIFSIGNALED(s_right))
		state->last_exit_code = 128 + WTERMSIG(s_right);
}

void	kill_first_process(t_shell_state *state, int fd[2], pid_t left)
{
	kill(left, SIGTERM);
	waitpid(left, NULL, 0);
	return (handle_fork_error(state, fd));
}
