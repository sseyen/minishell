/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_pwd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 16:41:24 by danslav1e         #+#    #+#             */
/*   Updated: 2025/10/10 22:40:52 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int built_in_pwd(t_node *node)
{
    char *pwd;

    if (node->argc != 1)
    {
        error_msg("pwd","too many arguments");
        return (FAILURE);
    }
    pwd = getcwd(NULL, 0);
    if (!pwd)
    {
        return(error_msg_cmd("pwd: error retrieving current directory", "getcwd: cannot access parent directories", strerror(errno), FAILURE));
    }
    ft_putstr_fd(pwd, STDOUT_FILENO);
    write(STDOUT_FILENO, "\n", 1);
    free(pwd);
    return (SUCCESS);
}
