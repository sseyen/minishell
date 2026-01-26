/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expr_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 12:02:00 by alisseye          #+#    #+#             */
/*   Updated: 2026/01/26 01:52:07 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * @brief
 * Appends a redirect to node's redirect array.
 */
static int	append_redirect(t_parser *p, t_node *node, size_t *ri)
{
	if (fill_redirect(p, &node->redirects[*ri]) != 0)
		return (1);
	(*ri)++;
	node->redirects_count = *ri;
	return (0);
}

/**
 * @brief
 * Parses redirects appearing after command/subshell.
 * Allocates redirect array if needed.
 */
int	parse_suffix_redirects(t_parser *p, t_node *node)
{
	size_t	ri;
	size_t	cap;

	ri = node->redirects_count;
	while (p->idx < p->len && is_redir(p->tokens[p->idx].type))
	{
		if (!node->redirects)
		{
			cap = (p->len - p->idx) + 1;
			node->redirects = ft_calloc(cap, sizeof(t_redirect));
			if (!node->redirects)
				return (1);
		}
		if (append_redirect(p, node, &ri) != 0)
			return (1);
	}
	return (0);
}

/**
 * @brief
 * Creates a binary node (PIPE/AND/OR) with left and right children.
 */
static t_node	*join_binary(t_node *left, t_node *right, t_node_type type)
{
	t_node	*node;

	node = new_node(type);
	if (!node)
		return (free_ast(left), free_ast(right), NULL);
	node->left = left;
	node->right = right;
	return (node);
}

/**
 * @brief
 * Parses pipe expressions (cmd1 | cmd2 | ...).
 */
t_node	*parse_pipe(t_parser *p)
{
	t_node	*left;
	t_node	*right;

	left = parse_primary(p);
	if (!left)
		return (NULL);
	while (p->idx < p->len && p->tokens[p->idx].type == TOKEN_PIPE)
	{
		p->idx++;
		right = parse_primary(p);
		if (!right)
			return (free_ast(left), NULL);
		left = join_binary(left, right, NODE_PIPE);
		if (!left)
			return (NULL);
	}
	return (left);
}

/**
 * @brief
 * Parses AND expressions (cmd1 && cmd2 && ...).
 */
t_node	*parse_and(t_parser *p)
{
	t_node	*left;
	t_node	*right;

	left = parse_pipe(p);
	if (!left)
		return (NULL);
	while (p->idx < p->len && p->tokens[p->idx].type == TOKEN_AND)
	{
		p->idx++;
		right = parse_pipe(p);
		if (!right)
			return (free_ast(left), NULL);
		left = join_binary(left, right, NODE_AND);
		if (!left)
			return (NULL);
	}
	return (left);
}
