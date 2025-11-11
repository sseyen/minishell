/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 23:13:23 by danslav1e         #+#    #+#             */
/*   Updated: 2025/11/06 20:29:47 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** Добавляет новую переменную окружения (через realloc).
*/
static int	add_env_var(t_shell_state *state, char *var)
{
	char	**new_envp;
	int		count;
	int		i;

	count = 0;
	while (state->envp[count])
		count++;
	new_envp = (char **)malloc(sizeof(char *) * (count + 2));
	if (!new_envp)
		return (FAILURE); // Need error handling, exit the program, malloc error
	i = 0;
	while (i < count)
	{
		new_envp[i] = state->envp[i];
		i++;
	}
	new_envp[i] = ft_strdup(var);
	if (!new_envp[i])
	{
		return (FAILURE); // Malloc error, exit the program
	}
	new_envp[i + 1] = NULL;
	free(state->envp); // Освобождаем старый массив указателей
	state->envp = new_envp;
	return (SUCCESS);
}

/*
** "Устанавливает" (обновляет или добавляет) переменную.
** Это 'set_env', который используется в 'cd'.
*/
int	set_env_var(t_shell_state *state, char *var)
{
	char	*key;
	int		index;

	key = get_key_from_var(var);
	if (!key)
		return (FAILURE); // Malloc error, exit the program
	index = find_env_var_index(state->envp, key);
	free(key);
	if (index != -1 && ft_strchr(var, "="))
	{
		if (ft_strchr(var, "="))
		{
			// Нашли, обновляем
			free(state->envp[index]);
			state->envp[index] = ft_strdup(var);
			if (state->envp[index])
				return (SUCCESS);
			else
				return (FAILURE); // Exit the program, malloc error
		}
		else
			return (SUCCESS);
	}
	else
	{
		// Не нашли, добавляем
		return (add_env_var(state, var));
	}
}

/*
** Обрабатывает 'export' с аргументами.
*/
static int	handle_export_args(t_node *node, t_shell_state *state)
{
	int		i;
	int		exit_code;
	char	*arg;

	i = 1;
	exit_code = SUCCESS;
	while (i < node->argc)
	{
		arg = node->argv[i];
		if (!is_valid_identifier(arg))
		{
			exit_code = export_error(arg); // заменить на универсальное сообщение об ошибке
		}
		else
		{
			// Пытаемся добавить/обновить
			if (set_env_var(state, arg) == FAILURE)
				exit_code = FAILURE; // Exit the program if malloc error
		}
		i++;
	}
	return (exit_code);
}

/*
** Главная функция built_in_export.
*/
int	built_in_export(t_node *node, t_shell_state *state)
{
	if (node->argc == 1)
		return (print_sorted_env(state));
	else
		return (handle_export_args(node, state));
}