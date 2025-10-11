/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 16:34:42 by danslav1e         #+#    #+#             */
/*   Updated: 2025/10/09 23:10:45 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// check if command is built_in or not
bool is_built_in(t_node *node)
{
    if (ft_strncmp(node->argv[0], "echo", 5) == 0)
        return (true);
    else if (ft_strncmp(node->argv[0], "cd", 3) == 0)
        return (true);
    else if (ft_strncmp(node->argv[0], "pwd", 4) == 0)
        return (true);
    else if (ft_strncmp(node->argv[0], "export", 7) == 0)
        return (true);
    else if (ft_strncmp(node->argv[0], "unset", 6) == 0)
        return (true);
    else if (ft_strncmp(node->argv[0], "env", 4) == 0)
        return (true);
    else if (ft_strncmp(node->argv[0], "exit", 5) == 0)
        return (true);
    return (false);
}

void execute_built_in(t_node *node, t_shell_state *state)
{
    if (!apply_redirects(node))
    {
        state->last_exit_code = 1;
        return ;
    }
    if (ft_strncmp(node->argv[0], "echo", 5) == 0)
        state->last_exit_code = built_in_echo(node);
    else if (ft_strncmp(node->argv[0], "cd", 3) == 0)
        state->last_exit_code = built_in_cd(node);
    else if (ft_strncmp(node->argv[0], "pwd", 4) == 0)
        state->last_exit_code = built_in_pwd();
    else if (ft_strncmp(node->argv[0], "export", 7) == 0)
        state->last_exit_code = built_in_export(node);
    else if (ft_strncmp(node->argv[0], "unset", 6) == 0)
        state->last_exit_code = built_in_unset(node);
    else if (ft_strncmp(node->argv[0], "env", 4) == 0)
        state->last_exit_code = built_in_env(node);
    else if (ft_strncmp(node->argv[0], "exit", 5) == 0)
        state->last_exit_code = built_in_exit(node);
    return (0);
}

//main executing function, going throught all the nodes of AST with recursion
void execute_ast(t_node *node, t_shell_state *state)
{
    if (!node)
        return ;
    if (node->type == NODE_CMD)
    {
        if (is_built_in(node))
            execute_built_in(node, state);
        else
            execute_bin(node, state);
    }
    else if (node->type == NODE_PIPE)
        execute_pipe(node, state);
    else if (node->type == NODE_SUBSHELL)
        execute_subshell(node, state);
    else if (node->type == NODE_OR)
        execute_or(node, state);
    else if (node->type == NODE_AND)
        execute_and(node, state);
}
