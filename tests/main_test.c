/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 05:15:32 by alisseye          #+#    #+#             */
/*   Updated: 2026/01/14 19:42:03 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

t_node	*create_ast_only(char *line, t_shell_state *state)
{
	t_token	*tokens;
	t_node	*root;
	int		ret;

	tokens = NULL;
	root = NULL;

	ret = tokenize(line, &tokens, state);
	if (ret != 0)
	{
		printf("Error: Tokenization failed with code %d\n", ret);
		return (NULL);
	}

	ret = build_ast(tokens, &root);
	if (ret != 0)
	{
		printf("Error: AST Build failed with code %d\n", ret);
		return (NULL);
	}

	return (root);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell_state	state;
	char			**env_copy;
	t_node			*ast;

	state.envp = NULL;
	state.token_tree = NULL;
	state.last_exit_code = 0;
	
	env_copy = init_env(envp);
	if (!env_copy)
		return (1);
	state.envp = env_copy;

	if (argc == 3 && ft_strncmp(argv[1], "print_ast", 10) == 0)
	{
		printf("Analyzing command: \033[1;33m%s\033[0m\n", argv[2]);
		
		ast = create_ast_only(argv[2], &state);

		if (ast)
		{
			print_ast(ast);
			free_ast(ast);
		}
	}
	else
	{
		if (argc < 2)
		{
			printf("Usage: %s <test_name> [args...]\n", argv[0]);
			return (1);
		}
		if (ft_strncmp(argv[1], "env", 4) == 0)
			return (test_init_env(argc, argv, envp));
		else if (ft_strncmp(argv[1], "count_tokens", 13) == 0)
			return (test_count_tokens(argc, argv));
		else if (ft_strncmp(argv[1], "fill_tokens", 12) == 0)
			return (test_fill_tokens(argc, argv));
		else if (ft_strncmp(argv[1], "expand_tokens", 14) == 0)
			return (test_expand_tokens(argc, argv, envp));
		else if (ft_strncmp(argv[1], "build_ast", 10) == 0)
			return (test_build_ast(argc, argv, envp));
		else if (ft_strncmp(argv[1], "pwd", 4) == 0)
			return (test_pwd(argc, argv, envp));
		else
			printf("Unknown test: %s\n", argv[1]);
	}

	free_env(state.envp);
	return (0);
	
	return (0);
}
