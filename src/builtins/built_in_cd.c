/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 13:21:00 by danslav1e         #+#    #+#             */
/*   Updated: 2025/12/04 02:24:18 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief
 * Updates $PWD and $OLDPWD in `state->envp` after a successful `chdir()`.
 */
int	update_pwd_oldpwd(t_shell_state *state, char *old_pwd)
{
	char	*new_pwd;

	if (old_pwd)
	{
		if (set_new_env_var(state, "OLDPWD", old_pwd) == FAILURE)
		{
			free(old_pwd);
			return (FAILURE);
		}
		free(old_pwd);
	}
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		return (error_msg("cd", "error retrieving current directory",
				"getcwd: cannot access parent directories", FAILURE));
	}
	if (set_new_env_var(state, "PWD", new_pwd) == FAILURE)
	{
		free(new_pwd);
		return (FAILURE);
	}
	free(new_pwd);
	return (SUCCESS);
}

/**
 * @brief
 * Determines which path to `chdir` to.
 * - `cd` -> $HOME
 * - `cd /path` -> /path
 * - `cd path` -> path
 *
 * @return
 * A literal pointer to $HOME (don't free) if argc == 1.
 * A malloc'd pointer (must be freed) if argc > 1.
 * NULL on error (e.g., $HOME not set or malloc fail).
 */
char	*get_cd_path(t_node *node, t_shell_state *state)
{
	char	*path;

	if (node->argc == 1)
	{
		path = find_env_var_value("HOME", state);
		if (!path)
		{
			error_msg("cd", NULL, "HOME not set", FAILURE);
			return (NULL);
		}
		if (*path == '\0')
			return (".");
		return (path);
	}
	else
	{
		path = ft_strdup(node->argv[1]);
		if (!path)
		{
			error_msg("cd", NULL, "malloc error", FAILURE);
			return (NULL);
		}
		return (path);
	}
}

/**
 * @brief
 * Built-in 'cd' command.
 * - 'cd' (no args): changes to $HOME.
 * - 'cd /path/to': changes to absolute path.
 * - 'cd relative/path': changes to relative path.
 * - Updates $PWD and $OLDPWD in `state->envp`.
 */
int	built_in_cd(t_node *node, t_shell_state *state)
{
	char	*old_pwd;
	char	*path_to_free;

	if (node->argc > 2)
		return (error_msg("cd", NULL, "too many arguments", FAILURE));
	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
	{
		error_msg("cd", "error retrieving current directory",
			"getcwd: cannot access parent directories", FAILURE);
		path_to_free = find_env_var_value("PWD", state);
		if (path_to_free)
		{
			old_pwd = ft_strdup(path_to_free);
			if (!old_pwd)
				return (error_msg("cd", NULL, "malloc error", FAILURE));
		}
	}
	return (built_in_cd2(node, state, old_pwd));
}

int	built_in_cd2(t_node *node, t_shell_state *state, char *old_pwd)
{
	char	*path;
	char	*path_to_free;

	path = get_cd_path(node, state);
	if (!path)
	{
		free(old_pwd);
		return (FAILURE);
	}
	if (node->argc > 1)
		path_to_free = path;
	else
		path_to_free = NULL;
	if (chdir(path) != 0)
	{
		free(old_pwd);
		if (path_to_free)
			free(path_to_free);
		return (error_msg("cd", path, NULL, FAILURE));
	}
	if (path_to_free)
		free(path_to_free);
	return (update_pwd_oldpwd(state, old_pwd));
}
