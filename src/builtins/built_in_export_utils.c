/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_export_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 21:01:15 by danslav1e         #+#    #+#             */
/*   Updated: 2026/01/14 22:49:40 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief
 * Sorts a string array alphabetically (bubble sort).
 */
void	sort_envp_copy(char **env_copy)
{
	int		i;
	int		j;
	char	*temp;
	int		count;

	count = 0;
	while (env_copy[count])
		count++;
	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strncmp(env_copy[j], env_copy[j + 1], -1) > 0)
			{
				temp = env_copy[j];
				env_copy[j] = env_copy[j + 1];
				env_copy[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

/**
 * @brief
 * Prints the sorted environment variables in `declare -x` format.
 * Handles 'export' with no arguments.
 */
int	sort_env(t_shell_state *state)
{
	char	**env_copy;
	int		count;
	int		i;

	count = 0;
	while (state->envp[count])
		count++;
	env_copy = (char **)malloc(sizeof(char *) * (count + 1));
	if (!env_copy)
		return (error_msg("export", NULL, "malloc error", FAILURE));
	i = 0;
	while (i < count)
	{
		env_copy[i] = state->envp[i];
		i++;
	}
	env_copy[count] = NULL;
	sort_envp_copy(env_copy);
	i = -1;
	while (env_copy[++i])
		if (print_method(env_copy, ft_strchr(env_copy[i], '='), i))
			return (FAILURE);
	free(env_copy);
	return (SUCCESS);
}

int	print_method(char **env_copy, char *equal_sign, int i)
{
	char	*key;

	if (equal_sign)
	{
		key = ft_substr(env_copy[i], 0, equal_sign - env_copy[i]);
		if (!key)
			return (free(env_copy), error_msg("export", NULL, "malloc error",
					FAILURE));
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(key, STDOUT_FILENO);
		ft_putstr_fd("=\"", STDOUT_FILENO);
		ft_putstr_fd(equal_sign + 1, STDOUT_FILENO);
		ft_putstr_fd("\"\n", STDOUT_FILENO);
		free(key);
	}
	else
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(env_copy[i], STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
	return (SUCCESS);
}

/**
 * @brief
 * Checks if 'str' is a valid identifier for an env variable.
 * (e.g., starts with letter or '_', contains only letters,
 * numbers, or '_').
 */
int	is_valid_identifier(char *str)
{
	int		i;
	char	c;

	if (!str || !*str)
		return (false);
	if (!(ft_isalpha(str[0]) || str[0] == '_'))
		return (false);
	i = 1;
	c = str[i];
	while (c && c != '=')
	{
		if (!(ft_isalnum(c) || c == '_'))
			return (false);
		i++;
		c = str[i];
	}
	return (true);
}
