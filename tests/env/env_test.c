/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_test.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 18:55:57 by alisseye          #+#    #+#             */
/*   Updated: 2025/10/11 19:36:07 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	run_test(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp; // test: get value by key...
}

int	main(int argc, char **argv, char **envp)
{
	char	**envp_copy;

	envp_copy = init_env(envp);
	if (argc == 1)
		print_env(envp_copy);
	else
		run_test(argc, argv, envp_copy);
	return (0);
}
