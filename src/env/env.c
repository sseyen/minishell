/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 18:36:30 by alisseye          #+#    #+#             */
/*   Updated: 2025/12/14 19:33:25 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	env_len(char **envp)
{
	size_t	i;

	i = 0;
	while (envp[i])
		i++;
	return (i);
}

static char	**alloc_empty_env(void)
{
	char	**envp_copy;

	envp_copy = malloc(sizeof(char *));
	if (!envp_copy)
		return (NULL);
	envp_copy[0] = NULL;
	return (envp_copy);
}

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

char	**init_env(char **envp)
{
	size_t	len;

	if (!envp)
		return (alloc_empty_env());
	len = env_len(envp);
	return (dup_env(envp, len));
}

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
