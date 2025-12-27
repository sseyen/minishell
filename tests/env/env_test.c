/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_test.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 18:55:57 by alisseye          #+#    #+#             */
/*   Updated: 2025/12/27 16:28:00 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

static int	test_init_basic(char **envp)
{
	char	**env_copy;

	print_test_header(0, "Basic environment initialization");
	printf("[EXEC] Initializing environment...\n");
	env_copy = init_env(envp);
	if (env_copy != NULL)
	{
		printf("[RESULT] Environment initialized successfully\n");
		print_result(1);
		free_env(env_copy);
		return (1);
	}
	printf("[RESULT] init_env returned NULL\n");
	print_result(0);
	return (0);
}

static int	test_valid_vars(char **envp)
{
	char	**env_copy;

	print_test_header(1, "Environment contains valid variables");
	printf("[EXEC] Checking environment pointer...\n");
	env_copy = init_env(envp);
	if (env_copy == NULL)
		return (0);
	if (env_copy[0] != NULL)
	{
		printf("[RESULT] Environment contains valid variables\n");
		print_result(1);
		free_env(env_copy);
		return (1);
	}
	printf("[RESULT] first env var is NULL\n");
	print_result(0);
	free_env(env_copy);
	return (0);
}

static int	test_find_variable(char **envp)
{
	char	**env_copy;
	int		idx;

	print_test_header(2, "Finding variables in environment");
	printf("[EXEC] Finding PATH variable...\n");
	env_copy = init_env(envp);
	if (env_copy == NULL)
		return (0);
	idx = find_env_var_index(env_copy, "PATH");
	printf("[RESULT] PATH ");
	if (idx != -1)
	{
		printf("found at index %d\n", idx);
	}
	else
	{
		printf("not found (might be unset)\n");
	}
	print_result(1);
	free_env(env_copy);
	return (1);
}

int	test_init_env(int argc, char **argv, char **envp)
{
	int	pass;

	(void)argv;
	(void)argc;
	printf("\n[INIT] Testing Environment Functions...\n");
	pass = test_init_basic(envp);
	pass += test_valid_vars(envp);
	pass += test_find_variable(envp);
	print_summary(3, pass);
	return (0);
}
