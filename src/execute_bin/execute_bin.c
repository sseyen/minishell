/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 17:19:01 by danslav1e         #+#    #+#             */
/*   Updated: 2025/11/11 18:22:08 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*find_in_path(char *cmd, t_shell_state *state)
{
	char	**paths;
	char	*path_env;
	char	*temp_path;
	char	*full_path;
	int		i;

	path_env = find_env_var_value("PATH", state);
	if (!path_env)
		return (NULL); // $PATH не установлен
	paths = ft_split(path_env, ':');
	if (!paths)
	{
		perror("minishell: malloc");
		exit(FAILURE);
	}
	i = 0;
	while (paths[i])
	{
		temp_path = ft_strjoin(paths[i], "/");
		if (!temp_path)
		{
			perror("minishell: malloc");
			exit(FAILURE);
		}
		full_path = ft_strjoin(temp_path, cmd);
		free(temp_path);
		if (!full_path)
		{
			perror("minishell: malloc");
			exit(FAILURE);
		}
		if (access(full_path, F_OK | X_OK) == 0)
		{
			// Нашли!
			free_split_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_split_array(paths);
	return (NULL);
}

/*
** Проверяет, является ли данный путь (абсолютный или относительный)
** исполняемым файлом.
** Выходит (exit) с ошибкой 126, если это директория или нет прав.
*/
static void	check_path_validity(char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
		{
			// Это директория
			error_msg_cmd(path, NULL, "is a directory", 126);
			exit(126);
		}
	}
	// Проверяем доступ (Существует?)
	if (access(path, F_OK) == -1)
	{
		// Файл не найден (для ./cmd или /bin/cmd)
		error_msg_cmd(path, NULL, strerror(errno), 127);
		exit(127);
	}
	if (access(path, X_OK) == -1)
	{
		// Файл существует, но нет прав на исполнение
		error_msg_cmd(path, NULL, strerror(errno), 126);
		exit(126);
	}
}

/*
** Основная функция запуска.
** Эта функция вызывается *только* в дочернем процессе.
** Она *никогда* не возвращает управление.
*/
void	execute_bin(t_node *node, t_shell_state *state)
{
	char	*cmd;
	char	*path;

	cmd = node->argv[0];
	if (!cmd || !*cmd)
		exit(SUCCESS);
	if (ft_strchr(cmd, '/'))
	{
		// Случай 1: Путь указан (e.g. "/bin/ls" or "./minishell")
		check_path_validity(cmd);
		path = cmd;
	}
	else
	{
		// Случай 2: Имя команды (e.g. "ls") - ищем в $PATH
		path = find_in_path(cmd, state);
		if (!path)
		{
			error_msg_cmd(cmd, NULL, "command not found", 127);
			exit(127);
		}
	}
	// Если мы здесь, 'path' - это валидный, исполняемый файл
	execve(path, node->argv, state->envp);
	// Если execve ВЕРНУЛСЯ - это 100% ошибка
	error_msg_cmd(cmd, NULL, strerror(errno), 126);
	if (path != cmd)
		free(path); // Освобождаем path, только если мы его аллоцировали
	exit(126);
}