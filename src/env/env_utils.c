/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 19:09:45 by danslav1e         #+#    #+#             */
/*   Updated: 2025/11/06 19:20:29 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** Извлекает 'KEY' из строки 'KEY=VALUE'.
** Возвращает strdup, который нужно освободить.
*/
char	*get_key_from_var(char *var)
{
	char	*equal_sign;
	char	*key;

	equal_sign = ft_strchr(var, '=');
	if (equal_sign)
		key = ft_substr(var, 0, equal_sign - var);
	else
		key = ft_strdup(var);
	return (key);
}

/*
** Находит индекс переменной 'key' в 'envp'.
** Возвращает -1, если не найдено.
*/
int	find_env_var_index(char **envp, char *key)
{
	int		i;
	int		key_len;
	char	*env_key;

	i = 0;
	key_len = ft_strlen(key);
	while (envp[i])
	{
		env_key = get_key_from_var(envp[i]);
		if (env_key)
		{
			if (ft_strncmp(key, env_key, key_len + 1) == 0)
			{
				free(env_key);
				return (i);
			}
			free(env_key);
		}
        else
        {
            //stop program
        }
		i++;
	}
	return (-1);
}