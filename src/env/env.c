/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 18:36:30 by alisseye          #+#    #+#             */
/*   Updated: 2026/01/26 01:52:06 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief
 * Counts the number of environment variables.
 */
static size_t	env_len(char **envp)
{
	size_t	i;

	i = 0;
	while (envp[i])
		i++;
	return (i);
}

/**
 * @brief
 * Allocates an empty environment (just NULL terminator).
 */
static char	**alloc_empty_env(void)
{
	char	**envp_copy;

	envp_copy = malloc(sizeof(char *));
	if (!envp_copy)
		return (NULL);
	envp_copy[0] = NULL;
	return (envp_copy);
}

/**
 * @brief
 * Duplicates an environment array.
 * Each string is copied with strdup.
 */
static char	**dup_env(char **envp, size_t len)
{
	char	**envp_copy;
	size_t	i;

	envp_copy = malloc((len + 1) * sizeof(char *));
	if (!envp_copy)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		envp_copy[i] = ft_strdup(envp[i]);
		if (!envp_copy[i])
		{
			while (i-- > 0)
				free(envp_copy[i]);
			free(envp_copy);
			return (NULL);
		}
		i++;
	}
	envp_copy[i] = NULL;
	return (envp_copy);
}

/**
 * @brief
 * Initializes a copy of the environment.
 *
 * @param envp Original environment from main.
 * @return Newly allocated environment copy or NULL on error.
 */
char	**init_env(char **envp)
{
	size_t	len;

	if (!envp)
		return (alloc_empty_env());
	len = env_len(envp);
	return (dup_env(envp, len));
}

/**
 * @brief
 * Frees all strings in the environment and the array itself.
 */
void	free_env(char **envp)
{
	size_t	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}
