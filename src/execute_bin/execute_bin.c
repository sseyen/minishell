/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_bin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 18:22:47 by danslav1e         #+#    #+#             */
/*   Updated: 2026/01/26 01:52:06 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief
 * Searches for an executable in PATH directories.
 *
 * @param cmd Command name to search for.
 * @param state Shell state containing envp.
 * @return Full path to executable or NULL if not found.
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

/**
 * @brief
 * Checks if command exists in a single PATH directory.
 *
 * @param state Shell state.
 * @param paths Array of PATH directories to free on success.
 * @param path Single directory to check.
 * @param cmd Command name.
 * @return Full path if found, NULL otherwise.
 */
char	*check_one_path(t_shell_state *state, char **paths, char *path,
		char *cmd)
{
	char	*temp_path;
	char	*full_path;

	temp_path = ft_strjoin(path, "/");
	if (!temp_path)
		malloc_exit_path(state, paths);
	full_path = ft_strjoin(temp_path, cmd);
	free(temp_path);
	if (!full_path)
		malloc_exit_path(state, paths);
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
 * Validates a path for execution.
 * Exits with error if path is directory, not found, or not executable.
 *
 * @param state Shell state.
 * @param path Path to validate.
 */
void	check_path_validity(t_shell_state *state, char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
		{
			state->last_exit_code = error_msg(path, NULL, "Is a directory",
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
 * Resolves command to its full path.
 * Returns cmd if it contains '/', otherwise searches PATH.
 */
static char	*resolve_cmd_path(char *cmd, t_shell_state *state)
{
	char	*path;

	if (ft_strchr(cmd, '/'))
	{
		check_path_validity(state, cmd);
		return (cmd);
	}
	path = find_in_path(cmd, state);
	if (!path)
	{
		state->last_exit_code = error_msg(cmd, NULL, "command not found", 127);
		exit_minishell(state);
	}
	return (path);
}

/**
 * @brief
 * Executes an external binary via execve.
 * Resolves path, sets up signals, and replaces process.
 *
 * @param node Command node containing argv.
 * @param state Shell state.
 */
void	execute_bin(t_node *node, t_shell_state *state)
{
	char	*cmd;
	char	*path;

	setup_signals_child();
	cmd = node->argv[0];
	if (!cmd || !*cmd)
	{
		state->last_exit_code = SUCCESS;
		exit_minishell(state);
	}
	path = resolve_cmd_path(cmd, state);
	execve(path, node->argv, state->envp);
	exit_failed_bin(state, cmd, path);
}
