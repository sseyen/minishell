/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 22:30:00 by alisseye          #+#    #+#             */
/*   Updated: 2026/01/26 01:52:06 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief
 * Reads heredoc input until delimiter or signal interrupt.
 * Writes lines to pipe for later execution.
 *
 * @param redir Redirect struct with delimiter.
 * @param fd Pipe file descriptors.
 * @param stdin_copy Saved stdin for signal recovery.
 * @return SUCCESS, FAILURE, or 130 on signal.
 */
static int	heredoc_read_loop(t_redirect *redir, int fd[2], int stdin_copy)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (g_signal == 130)
			return (heredoc_signal_cleanup(stdin_copy, fd, line));
		if (!line || is_heredoc_delimiter(redir, line))
			break ;
		if (write_heredoc_line(fd[1], line) == FAILURE)
			return (heredoc_write_error(stdin_copy, fd, line));
		free(line);
	}
	free(line);
	close(stdin_copy);
	close(fd[1]);
	redir->heredoc_fd = fd[0];
	return (SUCCESS);
}

/**
 * @brief
 * Creates pipe and reads heredoc content.
 * Sets up signals and delegates to read loop.
 */
static int	fill_heredoc(t_redirect *redir)
{
	int		fd[2];
	int		stdin_copy;

	if (pipe(fd) == -1)
		return (error_msg("pipe", NULL, strerror(errno), FAILURE), FAILURE);
	stdin_copy = dup(STDIN_FILENO);
	if (stdin_copy == -1)
		return (close(fd[0]), close(fd[1]), FAILURE);
	g_signal = 0;
	setup_signals_heredoc();
	return (heredoc_read_loop(redir, fd, stdin_copy));
}

/**
 * @brief
 * Processes all heredoc redirects for a single command node.
 */
static int	prepare_cmd_heredocs(t_node *node)
{
	size_t	i;
	int		ret;

	i = 0;
	while (i < node->redirects_count)
	{
		if (node->redirects[i].type == REDIRECT_HEREDOC)
		{
			ret = fill_heredoc(&node->redirects[i]);
			if (ret != SUCCESS)
				return (ret);
		}
		i++;
	}
	return (SUCCESS);
}

/**
 * @brief
 * Recursively prepares all heredocs in the AST.
 * Traverses all nodes to collect heredoc content before execution.
 *
 * @param node Root of AST subtree.
 * @return SUCCESS or error code.
 */
int	prepare_heredocs(t_node *node)
{
	int	ret;

	if (!node)
		return (SUCCESS);
	if (node->type == NODE_CMD)
	{
		ret = prepare_cmd_heredocs(node);
		if (ret != SUCCESS)
			return (ret);
	}
	ret = prepare_heredocs(node->left);
	if (ret != SUCCESS)
		return (ret);
	ret = prepare_heredocs(node->right);
	if (ret != SUCCESS)
		return (ret);
	ret = prepare_heredocs(node->child);
	if (ret != SUCCESS)
		return (ret);
	return (SUCCESS);
}
