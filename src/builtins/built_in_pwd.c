/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_pwd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 16:41:24 by danslav1e         #+#    #+#             */
/*   Updated: 2025/11/15 02:21:51 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief
 * Built-in 'pwd' command.
 * Prints the current working directory.
 * Per the subject, it does not accept any options.
 */
int	built_in_pwd(t_node *node)
{
	char	*pwd;

	if (node->argc != 1)
		return (error_msg("pwd", NULL, "too many arguments", FAILURE));
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (error_msg("pwd", NULL, NULL, FAILURE));
	ft_putendl_fd(pwd, STDOUT_FILENO);
	free(pwd);
	return (SUCCESS);
}
