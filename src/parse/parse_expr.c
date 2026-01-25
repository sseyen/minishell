/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 18:55:40 by alisseye          #+#    #+#             */
/*   Updated: 2026/01/25 00:19:57 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

static t_node	*parse_paren(t_parser *p)
{
	t_node	*child;
	t_node	*node;

	p->idx++;
	child = parse_expr(p);
	if (!child || p->idx >= p->len || p->tokens[p->idx].type != TOKEN_RPAREN)
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

/**
 * @brief Parse a command or parenthesized subshell.
 *
 * @param p Parser state.
 *
 * @return Subtree or NULL on syntax/alloc failure.
 */
t_node	*parse_primary(t_parser *p)
{
	t_node	*node;

	if (p->idx >= p->len)
		return (NULL);
	if (p->tokens[p->idx].type == TOKEN_LPAREN)
		return (parse_paren(p));
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
/**
 * @brief Top-level expression parser: && and || share precedence,
	left-to-right.
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
	t_token	tok;

	left = parse_pipe(p);
	if (!left)
		return (NULL);
	while (p->idx < p->len && (p->tokens[p->idx].type == TOKEN_AND
			|| p->tokens[p->idx].type == TOKEN_OR))
	{
		tok = p->tokens[p->idx++];
		right = parse_pipe(p);
		if (!right)
			return (free_ast(left), NULL);
		if (tok.type == TOKEN_AND)
			node = new_node(NODE_AND);
		else
			node = new_node(NODE_OR);
		if (!node)
			return (free_ast(left), free_ast(right), NULL);
		node->left = left;
		node->right = right;
		left = node;
	}
	return (left);
}
