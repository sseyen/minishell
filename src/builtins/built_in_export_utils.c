/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_export_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 19:29:34 by danslav1e         #+#    #+#             */
/*   Updated: 2025/11/06 20:48:24 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	sort_envp_copy(char **env_copy)
{
	int		i;
	int		j;
	char	*temp;
	int		count;

	count = 0;
	while (env_copy[count])
		count++;
	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strncmp(env_copy[j], env_copy[j + 1], -1) > 0)
			{
				temp = env_copy[j];
				env_copy[j] = env_copy[j + 1];
				env_copy[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

/*
** Создает, сортирует и печатает копию envp.
*/
static int	print_sorted_env(t_shell_state *state)
{
	char	**env_copy;
	int		count;
	int		i;
	char	*equal_sign;
	char	*key;

	count = 0;
	while (state->envp[count])
		count++;
	env_copy = (char **)malloc(sizeof(char *) * (count + 1));
	if (!env_copy)
		return (FAILURE); // Need error handling
	i = 0;
	while (i < count)
	{
		env_copy[i] = state->envp[i];
		i++;
	}
	env_copy[count] = NULL;
	sort_envp_copy(env_copy);
	i = 0;
	while (env_copy[i])
	{
		// Печатаем в формате declare -x KEY="VALUE"
		// (Эта часть немного сложная, чтобы уложиться в 25 строк,
		// но здесь основная логика)
		equal_sign = ft_strchr(env_copy[i], '=');
		if (equal_sign)
		{
			key = ft_substr(env_copy[i], 0, equal_sign - env_copy[i]);
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putstr_fd(key, STDOUT_FILENO);
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(equal_sign + 1, STDOUT_FILENO);
			ft_putstr_fd("\"\n", STDOUT_FILENO);
			free(key);
		}
		else
		{
			// Для переменных без "=", e.g. 'export VAR'
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putstr_fd(env_copy[i], STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		i++;
	}
	free(env_copy); // Освобождаем только массив указателей, не сами строки
	return (SUCCESS);
}

/*
** Выводит ошибку о невалидном идентификаторе.
*/
static int	export_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	return (FAILURE);
}

/*
** Проверяет, является ли 'str' валидным идентификатором для 'export'.
*/
int	is_valid_identifier(char *str)
{
	int i;
	char c;

	if (!str || !*str)
		return (0);
	if (!(ft_isalpha(str[0]) || str[0] == '_'))
		return (0);
	i = 1;
	c = str[i];
	while (c && c != '=')
	{
		if (!(ft_isalnum(c) || c == '_'))
			return (0);
		i++;
		c = str[i];
	}
	return (1);
}