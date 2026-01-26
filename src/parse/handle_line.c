/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 00:00:00 by danslav1e         #+#    #+#             */
/*   Updated: 2026/01/26 01:52:06 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief
 * Prepares heredocs and executes the AST.
 * Sets exit code to 130 if heredoc was interrupted.
 *
 * @param state Shell state containing the AST.
 * @return 0 on success, 130 if interrupted, or error code.
 */
static int	process_ast(t_shell_state *state)
{
	int	ret;

	ret = prepare_heredocs(state->token_tree);
	if (ret == 0)
		execute_ast(state->token_tree, state);
	else if (ret == 130)
		state->last_exit_code = 130;
	return (ret);
}

/**
 * @brief
 * Tokenizes, parses, and executes a command line.
 *
 * @param line The input command line.
 * @param state Shell state.
 * @return 0 on success, or error/exit code.
 */
int	handle_line(char *line, t_shell_state *state)
{
	t_token	*tokens;
	int		ret;

	ret = tokenize(line, &tokens, state);
	if (ret != 0)
		return (ret);
	ret = build_ast(tokens, &state->token_tree);
	if (ret != 0)
	{
		if (state->token_tree)
			free_ast(state->token_tree);
		state->token_tree = NULL;
		return (ret);
	}
	if (state->token_tree)
		ret = process_ast(state);
	if (state->token_tree)
		free_ast(state->token_tree);
	state->token_tree = NULL;
	return (ret);
}
