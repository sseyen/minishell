/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 18:36:30 by alisseye          #+#    #+#             */
/*   Updated: 2025/10/11 18:56:24 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**init_env(char **envp)
{
	char	**envp_copy;
	size_t	i;

	i = 0;
	while (envp[i])
		i++;
	envp_copy = malloc((i + 1) * sizeof(char *));
	if (!envp_copy)
	{
		perror("envp_copy malloc");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (envp[i])
	{
		envp_copy[i] = ft_strdup(envp[i]);
		if (!envp_copy[i])
		{
			perror("envp_copy strdup");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	envp_copy[i] = NULL;
	return (envp_copy);
}

void	print_env(char **envp)
{
	size_t	i;

	i = 0;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
}
