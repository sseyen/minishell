/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 22:30:00 by alisseye          #+#    #+#             */
/*   Updated: 2025/12/14 18:38:47 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static bool	is_delimiter(t_redirect *redir, char *line)
{
	return (ft_strncmp(line, redir->target, ft_strlen(redir->target) + 1)
		== 0);
}

static int	write_heredoc_line(int fd, char *line)
{
	if (write(fd, line, ft_strlen(line)) == -1)
		return (error_msg("write", NULL, strerror(errno), FAILURE), FAILURE);
	if (write(fd, "\n", 1) == -1)
		return (error_msg("write", NULL, strerror(errno), FAILURE), FAILURE);
	return (SUCCESS);
}

/**
 * @brief Fill a heredoc into a pipe and store the read end in the redirect.
 *
 * @param redir Redirect descriptor with delimiter in `target`.
 *
 * @return SUCCESS on success, FAILURE on error.
 */
static int	fill_heredoc(t_redirect *redir)
{
	int		fd[2];
	char	*line;

	if (pipe(fd) == -1)
		return (error_msg("pipe", NULL, strerror(errno), FAILURE), FAILURE);
	while (1)
	{
		line = readline("> ");
		if (!line || is_delimiter(redir, line))
			break ;
		if (write_heredoc_line(fd[1], line) == FAILURE)
		{
			free(line);
			close(fd[0]);
			close(fd[1]);
			return (FAILURE);
		}
		free(line);
	}
	free(line);
	close(fd[1]);
	redir->heredoc_fd = fd[0];
	return (SUCCESS);
}

/**
 * @brief Prepare heredoc pipes for a command node.
 *
 * @param node Command node whose redirects are inspected.
 *
 * @return SUCCESS on success, FAILURE on error.
 */
static int	prepare_cmd_heredocs(t_node *node)
{
	size_t	i;

	i = 0;
	while (i < node->redirects_count)
	{
		if (node->redirects[i].type == REDIRECT_HEREDOC)
		{
			if (fill_heredoc(&node->redirects[i]) == FAILURE)
				return (FAILURE);
		}
		i++;
	}
	return (SUCCESS);
}

/**
 * @brief Recursively prepare heredocs across the AST.
 *
 * @param node Root of the AST/subtree.
 *
 * @return SUCCESS on success, FAILURE on error.
 */
int	prepare_heredocs(t_node *node)
{
	if (!node)
		return (SUCCESS);
	if (node->type == NODE_CMD)
	{
		if (prepare_cmd_heredocs(node) == FAILURE)
			return (FAILURE);
	}
	if (prepare_heredocs(node->left) == FAILURE)
		return (FAILURE);
	if (prepare_heredocs(node->right) == FAILURE)
		return (FAILURE);
	if (prepare_heredocs(node->child) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}
