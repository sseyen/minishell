/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 18:25:00 by alisseye          #+#    #+#             */
/*   Updated: 2026/01/26 01:52:07 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parse/parse.h"

/**
 * @brief
 * Frees redirect array, closes heredoc file descriptors.
 */
static void	free_redirects(t_redirect *redir, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		if (redir[i].heredoc_fd != -1)
			close(redir[i].heredoc_fd);
		if (redir[i].target)
			free(redir[i].target);
		i++;
	}
}

/**
 * @brief
 * Recursively frees an AST and all its resources.
 * Frees argv, redirects, and child nodes.
 *
 * @param node Root node to free.
 */
void	free_ast(t_node *node)
{
	size_t	i;

	if (!node)
		return ;
	if (node->left)
		free_ast(node->left);
	if (node->right)
		free_ast(node->right);
	if (node->child)
		free_ast(node->child);
	i = 0;
	while (node->argv && node->argv[i])
	{
		free(node->argv[i]);
		i++;
	}
	if (node->argv)
		free(node->argv);
	if (node->redirects)
	{
		free_redirects(node->redirects, node->redirects_count);
		free(node->redirects);
	}
	free(node);
}
