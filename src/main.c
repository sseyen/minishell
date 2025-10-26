/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:07:23 by alisseye          #+#    #+#             */
/*   Updated: 2025/10/26 02:48:41 by alisseye         ###   ########.fr       */
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
		if (state->first_node)
			free_tree(state->first_node);
	}
	exit(exit_code);
}

int	handle_line(char *line, t_shell_state *state)
{
	t_token	*tokens;

	if (tokenize(line, &tokens) != 0)
		return (1);
	if (build_tree(tokens, state->first_node) != 0)
	{
		if (state->first_node)
			free_tree(state->first_node);
		state->first_node = NULL;
		state->token_tree = NULL;
		return (1);
	}
	if (state->first_node)
		execute_tree(state);
	if (state->first_node)
		free_tree(state);
	state->first_node = NULL;
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
	state.first_node = NULL;
	state.token_tree = NULL;
	state.last_exit_code = 0;
	envp_copy = init_env(envp);
	if (!envp_copy)
		exit_minishell(&state, 1);
	state.envp = envp_copy;
	return (minishell_loop(&state));
}
