/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 16:41:28 by danslav1e         #+#    #+#             */
/*   Updated: 2025/11/06 19:21:44 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void remove_var(int index, t_shell_state *state)
{
    while (state->envp[index])
    {
        state->envp[index] = state->envp[index + 1];
        index++;
    }
}

int built_in_unset(t_node *node, t_shell_state *state) //to create testing data
{
    int index;
    int i;

    i = 1;
    index = 0;
    while (i < node->argc)
    {
        index = find_env_var_index(state->envp, node->argv[i]);
        if (index != -1)
            remove_var(index, state);
        i++;
    }
    return (SUCCESS);
}
