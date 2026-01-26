/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 00:00:00 by danslav1e         #+#    #+#             */
/*   Updated: 2026/01/26 01:52:06 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief
 * Checks if line matches the heredoc delimiter.
 */
bool	is_heredoc_delimiter(t_redirect *redir, char *line)
{
	return (ft_strncmp(line, redir->target, ft_strlen(redir->target) + 1)
		== 0);
}

/**
 * @brief
 * Writes a line to heredoc pipe with newline appended.
 *
 * @return SUCCESS or FAILURE.
 */
int	write_heredoc_line(int fd, char *line)
{
	if (write(fd, line, ft_strlen(line)) == -1)
		return (error_msg("write", NULL, strerror(errno), FAILURE), FAILURE);
	if (write(fd, "\n", 1) == -1)
		return (error_msg("write", NULL, strerror(errno), FAILURE), FAILURE);
	return (SUCCESS);
}

/**
 * @brief
 * Cleanup handler when heredoc is interrupted by signal.
 * Restores stdin, closes pipe, frees line.
 *
 * @return 130 (signal exit code).
 */
int	heredoc_signal_cleanup(int stdin_copy, int fd[2], char *line)
{
	dup2(stdin_copy, STDIN_FILENO);
	close(stdin_copy);
	close(fd[0]);
	close(fd[1]);
	free(line);
	return (130);
}

/**
 * @brief
 * Cleanup handler when heredoc write fails.
 * Closes file descriptors and frees resources.
 *
 * @return FAILURE.
 */
int	heredoc_write_error(int stdin_copy, int fd[2], char *line)
{
	free(line);
	close(stdin_copy);
	close(fd[0]);
	close(fd[1]);
	return (FAILURE);
}
