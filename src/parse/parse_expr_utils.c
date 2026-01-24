/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expr_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 12:02:00 by alisseye          #+#    #+#             */
/*   Updated: 2026/01/24 12:02:00 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

static int	append_redirect(t_parser *p, t_node *node, size_t *ri)
{
	if (fill_redirect(p, &node->redirects[*ri]) != 0)
		return (1);
	(*ri)++;
	node->redirects_count = *ri;
	return (0);
}

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
