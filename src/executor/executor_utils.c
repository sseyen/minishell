/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 00:00:00 by danslav1e         #+#    #+#             */
/*   Updated: 2026/01/26 01:52:06 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief
 * Handles signal termination of child process.
 * Sets exit code to 128+signal and prints appropriate message.
 *
 * @param status Exit status from waitpid.
 * @param state Shell state.
 */
void	handle_child_signal(int status, t_shell_state *state)
{
	int	sig;

	sig = WTERMSIG(status);
	state->last_exit_code = 128 + sig;
	if (sig == SIGINT)
		write(STDOUT_FILENO, "\n", 1);
	else if (sig == SIGQUIT)
		ft_putendl_fd("Quit: 3", STDOUT_FILENO);
}

/**
 * @brief
 * Dispatches to the appropriate built-in command handler.
 *
 * @param node Command node with argv.
 * @param state Shell state.
 */
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
