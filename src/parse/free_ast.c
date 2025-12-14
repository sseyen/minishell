/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 18:25:00 by alisseye          #+#    #+#             */
/*   Updated: 2025/12/14 18:28:53 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

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
