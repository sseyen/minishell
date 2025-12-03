/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:07:23 by alisseye          #+#    #+#             */
/*   Updated: 2025/11/21 13:04:25 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_minishell(t_shell_state *state, int exit_code)
{
	size_t	i;

	if (state)
	{
		if (state->envp)
			free_env(state->envp);
		if (state->token_tree)
			free_tree(state->token_tree);
	}
	exit(exit_code);
}

int	handle_line(char *line, t_shell_state *state)
{
	t_token	*tokens;

	if (tokenize(line, &tokens, state) != 0)
		return (1);
	if (build_tree(tokens, state->token_tree) != 0)
	{
		if (state->token_tree)
			free_tree(state->token_tree);
		state->token_tree = NULL;
		return (1);
	}
	if (state->token_tree)
		execute_tree(state);
	if (state->token_tree)
		free_tree(state->token_tree);
	state->token_tree = NULL;
	return (0);
}

int	minishell_loop(t_shell_state *state)
{
	char	*line;

	while (1)
	{
		line = readline("minishell$ ");
		if (*line)
			add_history(line);
		if (handle_line(line, state) != 0)
			state->last_exit_code = 1;
		free(line);
	}
	return (state->last_exit_code);
}

int	main(int argc, char **argv, char **envp)
{
	char			**envp_copy;
	t_shell_state	state;

	(void)argc;
	(void)argv;
	state.envp = NULL;
	state.token_tree = NULL;
	state.last_exit_code = 0;
	envp_copy = init_env(envp);
	if (!envp_copy)
		exit_minishell(&state, 1);
	state.envp = envp_copy;
	return (minishell_loop(&state));
}
