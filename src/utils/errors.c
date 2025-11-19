/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 16:04:08 by danslav1e         #+#    #+#             */
/*   Updated: 2025/11/19 20:18:35 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief
 * A universal, malloc-free error messaging function.
 * Mirrors the `bash` format: "minishell: cmd: arg: message\n"
 *
 * @param cmd
 * The command name (e.g., "cd", "export").
 * @param arg
 * The argument that caused the error (e.g., "file.txt", "'1a'").
 * (Can be NULL).
 * @param custom_msg
 * A specific message (e.g., "command not found").
 * (Can be NULL).
 * @param exit_code
 * The exit code to return.
 *
 * @return int
 * Returns the `exit_code` (for `state->last_exit_code = error_msg(...)`).
 *
 * @note
 * If `custom_msg` is NULL, `strerror(errno)` will be used instead.
 */
int	error_msg(char *cmd, char *arg, char *custom_msg, int exit_code)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (cmd)
	{
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (arg)
	{
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (custom_msg)
		ft_putendl_fd(custom_msg, STDERR_FILENO);
	else
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
	return (exit_code);
}
