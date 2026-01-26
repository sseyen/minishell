/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:07:23 by alisseye          #+#    #+#             */
/*   Updated: 2026/01/26 01:52:06 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = 0;

/**
 * @brief
 * Checks if a line consists only of whitespace characters.
 *
 * @param line The input line to check.
 * @return 1 if empty/whitespace-only, 0 otherwise.
 */
static int	is_empty_line(char *line)
{
	size_t	i;

	i = 0;
	while (line[i] && is_whitespace(line[i]))
		i++;
	return (line[i] == '\0');
}

/**
 * @brief
 * Processes a single input line: adds to history, parses,
 * and executes. Updates exit code on failure.
 *
 * @param line The input line.
 * @param state Shell state.
 */
static void	process_line(char *line, t_shell_state *state)
{
	int	ret;

	if (*line)
		add_history(line);
	if (is_empty_line(line))
		return ;
	ret = handle_line(line, state);
	if (ret != 0)
		state->last_exit_code = ret;
}

/**
 * @brief
 * Main read-eval-print loop for the shell.
 * Reads lines with readline, handles signals, and processes input.
 *
 * @param state Shell state.
 * @return Last exit code when loop terminates.
 */
int	minishell_loop(t_shell_state *state)
{
	char	*line;

	while (1)
	{
		setup_signals_interactive();
		g_signal = 0;
		line = readline("minishell$ ");
		if (g_signal == 130)
		{
			state->last_exit_code = 130;
			g_signal = 0;
		}
		if (!line)
		{
			ft_putstr_fd("exit\n", STDOUT_FILENO);
			break ;
		}
		process_line(line, state);
		free(line);
	}
	return (state->last_exit_code);
}

/**
 * @brief
 * Initializes the shell state structure with default values.
 *
 * @param state Shell state to initialize.
 */
static void	init_state(t_shell_state *state)
{
	state->envp = NULL;
	state->token_tree = NULL;
	state->last_exit_code = 0;
	state->saved_fd[0] = -1;
	state->saved_fd[1] = -1;
	state->is_child = 0;
}

/**
 * @brief
 * Entry point of minishell.
 * Initializes environment, runs the main loop, and exits.
 */
int	main(int argc, char **argv, char **envp)
{
	char			**envp_copy;
	t_shell_state	state;

	(void)argc;
	(void)argv;
	init_state(&state);
	envp_copy = init_env(envp);
	if (!envp_copy)
		exit(FAILURE);
	state.envp = envp_copy;
	state.last_exit_code = minishell_loop(&state);
	exit_minishell(&state);
	return (0);
}
