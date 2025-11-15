/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 16:41:28 by danslav1e         #+#    #+#             */
/*   Updated: 2025/11/15 02:38:51 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief
 * Removes a variable from the `envp` array by shifting pointers.
 */
void	remove_var(int index, t_shell_state *state)
{
	free(state->envp[index]);
	while (state->envp[index + 1])
	{
		state->envp[index] = state->envp[index + 1];
		index++;
	}
	state->envp[index] = NULL;
}

/**
 * @brief
 * Built-in 'unset' command.
 * Removes one or more environment variables.
 * Validates variable names before unsetting.
 */
int	built_in_unset(t_node *node, t_shell_state *state)
{
	int	index;
	int	i;
	int	exit_code;

	i = 1;
	exit_code = SUCCESS;
	while (i < node->argc)
	{
		if (!is_valid_identifier(node->argv[i]))
		{
			exit_code = error_msg("unset", node->argv[i],
					"not a valid identifier", FAILURE);
			i++;
			continue ;
		}
		index = find_env_var_index(state->envp, node->argv[i]);
		if (index == -2)
			return (FAILURE);
		else if (index != -1)
			remove_var(index, state);
		i++;
	}
	return (exit_code);
}
