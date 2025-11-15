/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_bin_util.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 18:23:58 by danslav1e         #+#    #+#             */
/*   Updated: 2025/11/11 18:26:18 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** Освобождает память из-под массива строк (результат ft_split).
*/
void	free_split_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

/*
** Ищет переменную в envp (например, "PATH") и
** возвращает ее значение (строку *после* знака '=').
** Возвращает NULL, если не найдено.
*/
char	*find_env_var_value(char *key, t_shell_state *state)
{
	int		i;
	size_t	key_len;
	char	*env_var;

	if (!key || !state || !state->envp)
		return (NULL);
	key_len = ft_strlen(key);
	i = 0;
	while (state->envp[i])
	{
		env_var = state->envp[i];
		if (ft_strncmp(env_var, key, key_len) == 0)
		{
			if (env_var[key_len] == '=')
			{
				// Нашли! Возвращаем указатель на
				// первый символ *после* '='
				return (&env_var[key_len + 1]);
			}
		}
		i++;
	}
	return (NULL);
}
