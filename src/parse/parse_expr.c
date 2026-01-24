/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 18:55:40 by alisseye          #+#    #+#             */
/*   Updated: 2025/12/14 18:52:04 by alisseye         ###   ########.fr       */
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

static int	parse_suffix_redirects(t_parser *p, t_node *node)
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
 * @brief Parse a command or parenthesized subshell.
 *
 * @param p Parser state.
 *
 * @return Subtree or NULL on syntax/alloc failure.
 */
static t_node	*parse_primary(t_parser *p)
{
	t_node	*child;
	t_node	*node;

	if (p->idx >= p->len)
		return (NULL);
	if (p->tokens[p->idx].type == TOKEN_LPAREN)
	{
		p->idx++;
		child = parse_expr(p);
		if (!child || p->idx >= p->len
			|| p->tokens[p->idx].type != TOKEN_RPAREN)
			return (free_ast(child), NULL);
		p->idx++;
		node = new_node(NODE_SUBSHELL);
		if (!node)
			return (free_ast(child), NULL);
		node->child = child;
		if (parse_suffix_redirects(p, node) != 0)
			return (free_ast(node), NULL);
		return (node);
	}
	node = parse_command(p);
	if (!node)
		return (NULL);
	if (parse_suffix_redirects(p, node) != 0)
		return (free_ast(node), NULL);
	return (node);
}

/**
 * @brief Parse left-associative chains of commands joined by |.
 *
 * @param p Parser state.
 *
 * @return Resulting subtree or NULL on failure.
 */
static t_node	*parse_pipe(struct s_parser *p)
{
	t_node	*left;
	t_node	*right;
	t_node	*node;

	left = parse_primary(p);
	if (!left)
		return (NULL);
	while (p->idx < p->len && p->tokens[p->idx].type == TOKEN_PIPE)
	{
		p->idx++;
		right = parse_primary(p);
		if (!right)
			return (free_ast(left), NULL);
		node = new_node(NODE_PIPE);
		if (!node)
			return (free_ast(left), free_ast(right), NULL);
		node->left = left;
		node->right = right;
		left = node;
	}
	return (left);
}

/**
 * @brief Parse left-associative chains of commands joined by &&.
 *
 * @param p Parser state.
 *
 * @return Resulting subtree or NULL on failure.
 */
static t_node	*parse_and(struct s_parser *p)
{
	t_node	*left;
	t_node	*right;
	t_node	*node;

	left = parse_pipe(p);
	if (!left)
		return (NULL);
	while (p->idx < p->len && p->tokens[p->idx].type == TOKEN_AND)
	{
		p->idx++;
		right = parse_pipe(p);
		if (!right)
			return (free_ast(left), NULL);
		node = new_node(NODE_AND);
		if (!node)
			return (free_ast(left), free_ast(right), NULL);
		node->left = left;
		node->right = right;
		left = node;
	}
	return (left);
}

/**
 * @brief Top-level expression parser handling OR precedence.
 *
 * @param p Parser state.
 *
 * @return Root of the parsed tree or NULL on failure.
 */
t_node	*parse_expr(t_parser *p)
{
	t_node	*left;
	t_node	*right;
	t_node	*node;

	left = parse_and(p);
	if (!left)
		return (NULL);
	while (p->idx < p->len && p->tokens[p->idx].type == TOKEN_OR)
	{
		p->idx++;
		right = parse_and(p);
		if (!right)
			return (free_ast(left), NULL);
		node = new_node(NODE_OR);
		if (!node)
			return (free_ast(left), free_ast(right), NULL);
		node->left = left;
		node->right = right;
		left = node;
	}
	return (left);
}
