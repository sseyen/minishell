/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:07:23 by alisseye          #+#    #+#             */
/*   Updated: 2025/10/11 20:28:48 by alisseye         ###   ########.fr       */
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

int	minishell_loop(t_shell_state *state)
{
	char	*input;

	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
			exit_minishell(state, state->last_exit_code);
		if (input[0] != '\0')
			add_history(input);
		if (parse_and_execute(input, state) != 0)
		{
			free(input);
			exit_minishell(state, 1);
		}
		free(input);
	}
	return (0);
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
