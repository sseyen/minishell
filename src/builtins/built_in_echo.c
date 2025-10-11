/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 17:25:53 by danslav1e         #+#    #+#             */
/*   Updated: 2025/10/10 20:48:53 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// check if param is an option (-n, -nnnn)
bool is_option(const char *str)
{
    size_t i;

    if (ft_strlen(str) == 1)
        return (false);
    i = 1;
    if (str[0] == '-')
    {
        while (i < ft_strlen(str))
            if (str[i++] != 'n')
                return (false);
    }
    else
        return (false);
    return (true);
}

// built-in echo
int built_in_echo(t_node *node)
{
    size_t i;
    int new_line;

    if (node->argc == 1)
        return (SUCCESS);
    new_line = 1;
    i = 1;
    while (i < node->argc && is_option(node->argv[i]))
    {
        new_line = 0;
        i++;
    }
    while (i < node->argc)
    {
        write(STDOUT_FILENO, node->argv[i], ft_strlen(node->argv[i]));
        if (i != node->argc - 1)
            write(STDOUT_FILENO, " ", 1);
        i++;
    }
    if (new_line)
        write(STDOUT_FILENO, "\n", 1);
    return (SUCCESS);
}

// int main()
// {
//     t_node node;
//     // char *args[] = {"echo", "-n", "hello", "world"};
//     // char *args[] = {"echo", "-n", "-nnn", "hello", "world"};
//     // char *args[] = {"echo", "hello", "world"};
//     // char *args[] = {"echo", "-n", "-nnn", "hello", "-nnnn", "world"};
//     // char *args[] = {"echo", "-", "-nnkn", "hello", "-nnnn", "world"};
//     node.argc = 6;
//     node.argv = args;
//     built_in_echo(&node);
//     return (0);
// }