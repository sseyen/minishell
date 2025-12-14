/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 16:41:12 by danslav1e         #+#    #+#             */
/*   Updated: 2025/12/14 19:41:35 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief
 * Checks if adding the next digit ('c_digit') will cause a
 * `long long` overflow, given the current number 'n' and 'sign'.
 */
int	check_overflow(unsigned long long n, char c_digit, int sign)
{
	unsigned long long	cutoff;
	unsigned long long	cutlim;

	if (sign == 1)
	{
		cutoff = 922337203685477580;
		cutlim = 7;
	}
	else
	{
		cutoff = 922337203685477580;
		cutlim = 8;
	}
	if (n > cutoff || (n == cutoff \
		&& (long long unsigned int)(c_digit - '0') > cutlim))
		return (false);
	return (true);
}

/**
 * @brief
 * Checks if 'str' is a valid 'long long'.
 * Catches "exit test" (non-numeric) and "exit 999...999" (overflow).
 */
int	is_valid_llong(char *str)
{
	int					i;
	int					sign;
	unsigned long long	n;

	i = 0;
	sign = 1;
	n = 0;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (str[i] == '\0')
		return (false);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		if (check_overflow(n, str[i], sign))
			return (false);
		n = n * 10 + (str[i] - '0');
		i++;
	}
	return (true);
}

/**
 * @brief
 * This function should free all allocated memory for:
 * (1) The AST (t_node tree)
 * (2) The `envp` copy
 * (3) Any other allocated resources.
 * And also clear readline history.
 */
int	free_all_resources(t_shell_state *state)
{
	(void)state;
	return (state->last_exit_code);
}

/**
 * @brief
 * Converts a string to a long long.
 */
long long	ft_atoll(const char *str)
{
	long long	sign;
	long long	result;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -sign;
		str++;
	}
	while (ft_isdigit(*str))
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}

/**
 * @brief
 * Built-in 'exit' command. Exits the minishell.
 * Mimics bash behavior for arguments.
 */
int	built_in_exit(t_node *node, t_shell_state *state)
{
	unsigned char	exit_code;

	if (node->argc == 1)
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		free_all_resources(state);
		exit(state->last_exit_code);
	}
	if (!is_valid_llong(node->argv[1]))
	{
		return (error_msg("exit", node->argv[1], "numeric argument required",
				2));
	}
	if (node->argc > 2)
		return (error_msg("exit", NULL, "too many arguments", FAILURE));
	ft_putendl_fd("exit", STDOUT_FILENO);
	exit_code = (unsigned char)ft_atoll(node->argv[1]);
	free_all_resources(state);
	exit(exit_code);
}
