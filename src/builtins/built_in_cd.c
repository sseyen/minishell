/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 13:21:00 by danslav1e         #+#    #+#             */
/*   Updated: 2025/10/10 21:36:09 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int built_in_cd(t_node *node, t_shell_state *state) // Короче тут на свежую голову посмотреть, чтобы всё нормально работает, написать main с тестами
{
    char *path;
    char *newpwd;
    char *oldpwd;
    char* temp;

    if (node->argc > 2)
    {
        write(STDERR_FILENO, "minishell: cd: too many arguments\n", 34);
        return (FAILURE);
    }
    if (node->argc == 1)
    {
        write(STDERR_FILENO, "minishell: cd: no arguments\n", 29);
        return (FAILURE);
    }
    oldpwd = getcwd(NULL, 0);
    if (!oldpwd)
    {
        error_msg_cmd("cd: error retrieving current directory", "getcwd: cannot access parent directories", strerror(errno), FAILURE);
        oldpwd = ft_strdup(get_env("PWD")); // need function geting enviroment variable
        if (!oldpwd)
            error_msg("cd: error retrieving current directory", "PWD: environment variable doesn't exist");
    }
    path = node->argv[1];
    if (chdir(path) != 0)
    {
        free(oldpwd);
        return (error_msg_cmd("cd: error changing directory", "chdir: cannot change directory", strerror(errno), FAILURE));
    }
    newpwd = getcwd(NULL, 0);
    if (!newpwd)
    {
        error_msg_cmd("cd: error retrieving current directory", "getcwd: cannot access parent directories", strerror(errno), FAILURE);
        if (path[0] == '/')
            newpwd = ft_strdup(path);
        else
        {
            newpwd = ft_strdup(get_env("PWD"));
            if (newpwd)
            {
                temp = ft_strjoin(newpwd, "/");
                free(newpwd);
                newpwd = ft_strjoin(temp, path);
                free(temp);
            }
            else
                error_msg("cd: error retrieving current directory", "OLDPWD: environment variable doesn't exist");
        }
    }
    if (oldpwd)
    {
        set_env(state, "OLDPWD", oldpwd);
        free(oldpwd);
    }
    if (newpwd)
    {
        set_env(state, "PWD", newpwd);
        free(newpwd);
    }
    return (SUCCESS);
}
