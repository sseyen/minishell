/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 17:25:53 by danslav1e         #+#    #+#             */
/*   Updated: 2025/12/04 02:23:51 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief
 * Checks if a string is a valid '-n' option (e.g., -n, -nnn).
 *
 * @param str
 * The string to check.
 * @return
 * `true` if it's a valid '-n' option, `false` otherwise.
 */
bool	is_option(const char *str)
{
	size_t	i;

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

/**
 * @brief
 * Built-in 'echo' command.
 * Handles the '-n' flag.
 */
int	built_in_echo(t_node *node)
{
	size_t	i;
	int		new_line;

	new_line = 1;
	i = 1;
	while (i < node->argc && is_option(node->argv[i]))
	{
		new_line = 0;
		i++;
	}
	while (i < node->argc)
	{
		ft_putstr_fd(node->argv[i], STDOUT_FILENO);
		if (i != node->argc - 1)
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (new_line)
		ft_putchar_fd('\n', STDOUT_FILENO);
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