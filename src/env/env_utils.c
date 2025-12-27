/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 19:09:45 by danslav1e         #+#    #+#             */
/*   Updated: 2025/12/27 16:18:20 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief
 * Extracts the 'KEY' from a 'KEY=VALUE' string.
 *
 * @return
 * A malloc'd string containing the KEY. Must be freed.
 * Returns NULL on malloc failure.
 */
char	*get_key_from_var(char *var)
{
	char	*equal_sign;
	char	*key;

	equal_sign = ft_strchr(var, '=');
	if (equal_sign)
		key = ft_substr(var, 0, equal_sign - var);
	else
		key = ft_strdup(var);
	if (!key)
		error_msg("export", NULL, "malloc error", FAILURE);
	return (key);
}

/**
 * @brief
 * Finds the index of an environment variable 'key' in `envp`.
 *
 * @return
 * The index (0 or more) if found.
 * -1 if not found.
 * -2 on malloc error.
 */
int	find_env_var_index(char **envp, char *key)
{
	int		i;
	int		key_len;
	char	*env_key;

	i = 0;
	key_len = ft_strlen(key);
	while (envp[i])
	{
		env_key = get_key_from_var(envp[i]);
		if (!env_key)
			return (-2);
		if (ft_strncmp(key, env_key, key_len + 1) == 0)
		{
			free(env_key);
			return (i);
		}
		free(env_key);
		i++;
	}
	return (-1);
}

/**
 * @brief
 * Creates and sets a variable in envp (e.g., "PWD=/new/path").
 * Handles malloc errors gracefully.
 */
int	set_new_env_var(t_shell_state *state, char *key, char *value)
{
	char	*temp_str;
	char	*full_var;

	temp_str = ft_strjoin(key, "=");
	if (!temp_str)
		return (error_msg("cd", NULL, "malloc error", FAILURE));
	full_var = ft_strjoin(temp_str, value);
	free(temp_str);
	if (!full_var)
		return (error_msg("cd", NULL, "malloc error", FAILURE));
	if (set_env_var(state, full_var) == FAILURE)
	{
		free(full_var);
		return (FAILURE);
	}
	free(full_var);
	return (SUCCESS);
}

char	*get_env_value(char *key, char **envp)
{
	int		index;
	char	*equal;

	if (!key || !envp)
		return (NULL);
	index = find_env_var_index(envp, key);
	if (index < 0)
		return (NULL);
	equal = ft_strchr(envp[index], '=');
	if (!equal)
		return (ft_strdup(""));
	return (ft_strdup(equal + 1));
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
