/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_bin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 18:22:47 by danslav1e         #+#    #+#             */
/*   Updated: 2026/01/25 00:17:17 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief
 * Searches for the command `cmd` in the directories listed in $PATH.
 *
 * @return
 * A malloc'd string containing the full path to the executable.
 * NULL if not found (or $PATH is unset).
 * Exits the process with FAILURE on malloc error.
 */
char	*find_in_path(char *cmd, t_shell_state *state)
{
	char	**paths;
	char	*path_env;
	int		i;
	char	*path;

	path_env = find_env_var_value("PATH", state);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
	{
		state->last_exit_code = error_msg("malloc", NULL,
				"memory allocation failed", FAILURE);
		exit_minishell(state);
	}
	i = 0;
	while (paths[i])
	{
		path = check_one_path(state, paths, paths[i++], cmd);
		if (path)
			return (path);
	}
	free_split_array(paths);
	return (NULL);
}

char	*check_one_path(t_shell_state *state, char **paths, char *path,
		char *cmd)
{
	char	*temp_path;
	char	*full_path;

	temp_path = ft_strjoin(path, "/");
	if (!temp_path)
	{
		free_split_array(paths);
		state->last_exit_code = error_msg("malloc", NULL,
				"memory allocation failed", FAILURE);
		exit_minishell(state);
	}
	full_path = ft_strjoin(temp_path, cmd);
	free(temp_path);
	if (!full_path)
	{
		free_split_array(paths);
		state->last_exit_code = error_msg("malloc", NULL,
				"memory allocation failed", FAILURE);
		exit_minishell(state);
	}
	if (access(full_path, F_OK | X_OK) == 0)
	{
		free_split_array(paths);
		return (full_path);
	}
	free(full_path);
	return (NULL);
}

/**
 * @brief
 * Checks if a given absolute/relative path is valid and executable.
 * Exits with 126 or 127 on failure.
 */
void	check_path_validity(t_shell_state *state, char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
		{
			state->last_exit_code = error_msg(path, NULL, "is a directory",
					126);
			exit_minishell(state);
		}
	}
	if (access(path, F_OK) == -1)
	{
		state->last_exit_code = error_msg(path, NULL, strerror(errno), 127);
		exit_minishell(state);
	}
	if (access(path, X_OK) == -1)
	{
		state->last_exit_code = error_msg(path, NULL, strerror(errno), 126);
		exit_minishell(state);
	}
}

/**
 * @brief
 * Executes an external command using `execve`.
 * This function is called in a child process and never returns on success.
 */
void	execute_bin(t_node *node, t_shell_state *state)
{
	char	*cmd;
	char	*path;

	cmd = node->argv[0];
	if (!cmd || !*cmd)
	{
		state->last_exit_code = SUCCESS;
		exit_minishell(state);
	}
	if (ft_strchr(cmd, '/'))
	{
		check_path_validity(state, cmd);
		path = cmd;
	}
	else
	{
		path = find_in_path(cmd, state);
		if (!path)
		{
			state->last_exit_code = error_msg(cmd, NULL, "command not found",
					127);
			exit_minishell(state);
		}
	}
	execve(path, node->argv, state->envp);
	exit_failed_bin(state, cmd, path);
}

void	exit_failed_bin(t_shell_state *state, char *cmd, char *path)
{
	error_msg(cmd, NULL, strerror(errno), 126);
	if (path != cmd)
		free(path);
	state->last_exit_code = 126;
	exit_minishell(state);
}
