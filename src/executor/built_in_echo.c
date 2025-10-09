/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 17:25:53 by danslav1e         #+#    #+#             */
/*   Updated: 2025/10/09 22:48:11 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// built-in echo
int built_in_echo(t_node *node)
{
    int i;
    int new_line;

    new_line = 1;
    i = 1;
    while (node->argc > 1 && i < node->argc && is_option(node->argv[i]))
    {
        new_line = 0;
        i++;
    }
    while (node->argc > 1 && i < node->argc)
    {
        write(STDOUT_FILENO, node->argv[i], ft_strlen(node->argv[i]));
        if (i != node->argc - 1)
            write(STDOUT_FILENO, " ", 1);
    }
    if (new_line)
        write(STDOUT_FILENO, "\n", 1);
    return (EXIT_SUCCESS);
}

bool is_option(const char *str)
{
    int flag;
    int i;

    flag = 1;
    i = 1;
    if (str[0] == '-') // тут надо подправить, пока что херово работает (+ сразу сделать main с тестами)
        while (i < ft_strlen(str))
            if (str[i] && str[i] != 'n')
                return (false);
    else
        return (false);
    return (true);
}