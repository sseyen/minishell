/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 18:36:30 by alisseye          #+#    #+#             */
/*   Updated: 2025/10/11 20:02:14 by alisseye         ###   ########.fr       */
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
