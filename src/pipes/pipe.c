/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 17:22:44 by danslav1e         #+#    #+#             */
/*   Updated: 2025/11/10 22:05:29 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void create_left_child(t_shell_state *state, t_node *node, int fd[2])
{
    dup2(fd[1], STDOUT_FILENO);
    close(fd[1]);
    close(fd[0]);
    execute_ast(state, node->left);
    exit(state->last_exit_code);
}

void create_right_child(t_shell_state *state, t_node *node, int fd[2])
{
    dup2(fd[0], STDIN_FILENO);
    close(fd[1]);
    close(fd[0]);
    execute_ast(state, node->right);
    exit(state->last_exit_code);
}

void	execute_pipe(t_node *node, t_shell_state *state)
{
	int fd[2];
	pid_t left;
	pid_t right;
    int s_right;
    int s_left;

	if (pipe(fd) == -1)
    {
        perror("pipe");
        state->last_exit_code = FAILURE;
        return ;
    }
	left = fork();
    if (left == -1)
    {
        perror("fork");
        state->last_exit_code = FAILURE;
        close(fd[0]);
        close(fd[1]);
        return ;
    }
	if (left == 0)
		create_left_child(state, node, fd);
	right = fork();
    if (right == -1)
    {
        perror("fork");
        state->last_exit_code = FAILURE;
        close(fd[0]);
        close(fd[1]);
        return ;
    }
	if (right == 0)
		create_right_child(state, node, fd);
    close(fd[0]);
    close(fd[1]);
    waitpid(left, &s_left, 0);
    waitpid(right, &s_right, 0);
    if (WIFEXITED(s_right))
        state->last_exit_code = WEXITSTATUS(s_right);
    else if (WIFSIGNALED(s_right))
        state->last_exit_code = 128 + WTERMSIG(s_right);
}
