/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 05:15:32 by alisseye          #+#    #+#             */
/*   Updated: 2025/12/14 18:09:22 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	main(int argc, char **argv, char **envp)
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
	return (0);
}
