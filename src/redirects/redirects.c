/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 22:00:15 by danslav1e         #+#    #+#             */
/*   Updated: 2025/12/04 02:23:51 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief
 * Opens the target file with the correct flags based on redirect type.
 *
 * @param redir The redirect token.
 * @return The file descriptor, or -1 on error.
 */
int	create_redirect_fd(t_redirect *redir)
{
	int	fd;

	fd = -1;
	if (redir->type == REDIRECT_IN)
		fd = open(redir->target, O_RDONLY);
	else if (redir->type == REDIRECT_OUT)
		fd = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == REDIRECT_APPEND)
		fd = open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (redir->type == REDIRECT_HEREDOC)
		fd = redir->heredoc_fd;
	if (fd == -1)
	{
		error_msg(redir->target, NULL, strerror(errno), FAILURE);
	}
	return (fd);
}

/**
 * @brief
 * Iterates through all redirects in a node and applies them.
 *
 * Redirects are applied in order. The last one determines the final

 * stdin/stdout. Intermediate files are created/truncated but closed immediately.
 *
 * @return SUCCESS (0) or FAILURE (1).
 */
int	apply_redirects(t_node *node)
{
	size_t	i;
	int		fd;

	i = 0;
	while (i < node->redirects_count)
	{
		fd = create_redirect_fd(&node->redirects[i]);
		if (fd == -1)
			return (FAILURE);
		if (node->redirects[i].type == REDIRECT_IN
			|| node->redirects[i].type == REDIRECT_HEREDOC)
		{
			dup2(fd, STDIN_FILENO);
		}
		else if (node->redirects[i].type == REDIRECT_OUT
			|| node->redirects[i].type == REDIRECT_APPEND)
		{
			dup2(fd, STDOUT_FILENO);
		}
		close(fd);
		i++;
	}
	return (SUCCESS);
}

/**
 * @brief
 * Restores the original STDIN and STDOUT from saved file descriptors.
 * Used by built-ins running in the parent process.
 */
void	restore_stdio(int saved_stdin, int saved_stdout)
{
	if (saved_stdin != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}
