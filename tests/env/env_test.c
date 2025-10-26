/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_test.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 18:55:57 by alisseye          #+#    #+#             */
/*   Updated: 2025/10/26 05:34:12 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	test_init_env(int argc, char **argv, char **envp)
{
	char	**envp_copy;

	(void)argv;
	(void)argc;
	envp_copy = init_env(envp);
	if (envp_copy == NULL)
		return (1);
	print_env(envp_copy);
	free_env(envp_copy);
	return (0);
}
