/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 16:41:08 by danslav1e         #+#    #+#             */
/*   Updated: 2025/11/20 01:38:21 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief
 * Built-in 'env' command.
 * Prints all environment variables that have a value (i.e., contain an '=').
 * Per the subject, it does not accept any arguments or options.
 */
int	built_in_env(t_node *node, t_shell_state *state)
{
	int	i;

	if (node->argc != 1)
		return (error_msg("env", NULL, "too many arguments", FAILURE));
	if (!state->envp)
		return (SUCCESS);
	i = 0;
	while (state->envp[i])
	{
		if (ft_strchr(state->envp[i], '='))
			ft_putendl_fd(state->envp[i], STDOUT_FILENO);
		i++;
	}
	return (SUCCESS);
}

// int main(int argc, char **argv, char **envp)
// {
//     t_shell_state state;
//     t_node node;

//     if (!argc || !argv)
//         return (1);
//     node.argc = 1;
//     state.envp = envp;
//     built_in_env(&node, &state);
//     return (0);
// }
