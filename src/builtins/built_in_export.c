/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 01:40:40 by danslav1e         #+#    #+#             */
/*   Updated: 2025/12/14 19:42:06 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief
 * Adds a new environment variable to `state->envp`.
 * Re-allocates the `envp` array to be one size larger.
 */
int	add_env_var(t_shell_state *state, char *var)
{
	char	**new_envp;
	int		count;
	int		i;

	count = 0;
	while (state->envp[count])
		count++;
	new_envp = (char **)malloc(sizeof(char *) * (count + 2));
	if (!new_envp)
		return (error_msg("export", NULL, "malloc error", FAILURE));
	i = 0;
	while (i < count)
	{
		new_envp[i] = state->envp[i];
		i++;
	}
	new_envp[i] = ft_strdup(var);
	if (!new_envp[i])
		return (free(new_envp), error_msg("export", NULL, "malloc error",
				FAILURE));
	new_envp[i + 1] = NULL;
	free(state->envp);
	state->envp = new_envp;
	return (SUCCESS);
}

/**
 * @brief
 * Sets (updates an existing or adds a new) environment variable.
 */
int	set_env_var(t_shell_state *state, char *var)
{
	char	*key;
	int		index;

	key = get_key_from_var(var);
	if (!key)
		return (error_msg("export", NULL, "malloc error", FAILURE));
	index = find_env_var_index(state->envp, key);
	free(key);
	if (index == -2)
		return (FAILURE);
	else if (index == -1)
		return (add_env_var(state, var));
	else
	{
		if (!ft_strchr(var, '='))
			return (SUCCESS);
		free(state->envp[index]);
		state->envp[index] = ft_strdup(var);
		if (!state->envp[index])
			return (error_msg("export", NULL, "malloc error", FAILURE));
	}
	return (SUCCESS);
}

/**
 * @brief
 * Handles the 'export' command when called with arguments.
 */
int	handle_export_args(t_node *node, t_shell_state *state)
{
	size_t		i;
	int			exit_code;
	char		*arg;

	i = 1;
	exit_code = SUCCESS;
	while (i < node->argc)
	{
		arg = node->argv[i];
		if (!is_valid_identifier(arg))
			exit_code = error_msg("export", arg, "not a valid identifier",
					FAILURE);
		else if (set_env_var(state, arg) == FAILURE)
			return (FAILURE);
		i++;
	}
	return (exit_code);
}

/**
 * @brief
 * Built-in 'export' command.
 * - 'export' (no args): prints sorted env vars.
 * - 'export VAR=val' or 'export VAR': sets an env var.
 */
int	built_in_export(t_node *node, t_shell_state *state)
{
	if (node->argc == 1)
		return (sort_env(state));
	else
		return (handle_export_args(node, state));
}
