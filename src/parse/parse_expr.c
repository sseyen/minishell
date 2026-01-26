/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 18:55:40 by alisseye          #+#    #+#             */
/*   Updated: 2026/01/26 01:52:06 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * @brief
 * Parses a parenthesized subshell expression.
 * Creates a NODE_SUBSHELL with child expression.
 */
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
 * @brief
 * Parses a primary expression (command or subshell).
 * Handles suffix redirects.
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
 * @brief
 * Creates an AND or OR node connecting left and right subtrees.
 */
static t_node	*create_logic_node(t_token tok, t_node *left, t_node *right)
{
	t_node	*node;

	if (tok.type == TOKEN_AND)
		node = new_node(NODE_AND);
	else
		node = new_node(NODE_OR);
	if (!node)
		return (free_ast(left), free_ast(right), NULL);
	node->left = left;
	node->right = right;
	return (node);
}

/**
 * @brief
 * Parses full expression with && and || operators.
 * Builds AST with correct precedence.
 */
t_node	*parse_expr(t_parser *p)
{
	t_node	*left;
	t_node	*right;
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
		left = create_logic_node(tok, left, right);
		if (!left)
			return (NULL);
	}
	return (left);
}
