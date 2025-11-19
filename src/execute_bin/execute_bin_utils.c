/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_bin_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 18:23:58 by danslav1e         #+#    #+#             */
/*   Updated: 2025/11/19 19:03:25 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief
 * Frees a null-terminated array of strings (e.g., from ft_split).
 *
 * @param arr
 * The array to free.
 */
void	free_split_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

/**
 * @brief
 * Finds a variable in envp (e.g., "PATH") and returns
 * a pointer to its value (the string *after* the '=').
 *
 * @param key
 * The variable name (e.g., "PATH", "HOME").
 * @param state
 * The shell state containing `envp`.
 *
 * @return
 * A pointer to the value string (e.g., "/usr/bin:...")
 * or NULL if not found.
 *
 * @note
 * **DO NOT FREE** a non-NULL return value.
 * It is a pointer directly into `state->envp`.
 */
char	*find_env_var_value(char *key, t_shell_state *state)
{
	int		i;
	size_t	key_len;
	char	*env_var;

	if (!key || !state || !state->envp)
		return (NULL);
	key_len = ft_strlen(key);
	i = 0;
	while (state->envp[i])
	{
		env_var = state->envp[i];
		if (ft_strncmp(env_var, key, key_len) == 0)
		{
			if (env_var[key_len] == '=')
			{
				return (&env_var[key_len + 1]);
			}
		}
		i++;
	}
	return (NULL);
}
