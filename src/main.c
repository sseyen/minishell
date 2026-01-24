/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:07:23 by alisseye          #+#    #+#             */
/*   Updated: 2026/01/24 20:33:38 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	{
		ret = prepare_heredocs(state->token_tree);
		if (ret == 0)
			execute_ast(state->token_tree, state);
	}
	if (state->token_tree)
		free_ast(state->token_tree);
	state->token_tree = NULL;
	return (ret);
}

int	minishell_loop(t_shell_state *state)
{
	char	*line;
	int		ret;
	size_t	i;

	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (*line)
			add_history(line);
		i = 0;
		while (line[i] && is_whitespace(line[i]))
			i++;
		if (line[i] == '\0')
		{
			free(line);
			continue ;
		}
		ret = handle_line(line, state);
		if (ret != 0)
			state->last_exit_code = ret;
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
	state.saved_fd[0] = -1;
	state.saved_fd[1] = -1;
	envp_copy = init_env(envp);
	if (!envp_copy)
		exit(FAILURE);
	state.envp = envp_copy;
	return (minishell_loop(&state));
}
