/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 16:04:08 by danslav1e         #+#    #+#             */
/*   Updated: 2025/10/10 22:48:32 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int error_msg_cmd(const char *msg, const char* problem, const char* error, int rnb) // Переделать, чтобы сразу писало в strerror
{
    char *str;
    char *temp;

    str = ft_strdup("minishell:");
    if (msg)
    {
        temp = ft_strjoin(str, " ");
        free(str);
        str = ft_strjoin(temp, msg);
        free(temp);
    }
    if (problem)
    {
        temp = ft_strjoin(str, " ");
        free(str);
        str = ft_strjoin(temp, problem);
        free(temp);
    }
    if (error)
    {
        temp = ft_strjoin(str, " ");
        free(str);
        str = ft_strjoin(temp, error);
        free(temp);
    }
    ft_putstr_fd(str, STDERR_FILENO);
    write(STDERR_FILENO, "\n", 1);
    free(str);
    return (rnb);
}

void error_msg(const char *msg, const char* problem) // + переделать, чтобы было двоеточие после msg
{
    char *str;
    char *temp;

    str = ft_strdup("minishell:");
    if (msg)
    {
        temp = ft_strjoin(str, " ");
        free(str);
        str = ft_strjoin(temp, msg);
        free(temp);
    }
    if (problem)
    {
        temp = ft_strjoin(str, " ");
        free(str);
        str = ft_strjoin(temp, problem);
        free(temp);
    }
    ft_putstr_fd(str, STDERR_FILENO);
    write(STDERR_FILENO, "\n", 1);
    free(str);
}