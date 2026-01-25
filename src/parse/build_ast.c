/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 17:54:55 by alisseye          #+#    #+#             */
/*   Updated: 2026/01/25 00:19:21 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

static int	handle_parse_failure(t_token *tokens, t_node *tree,
		t_parser *parser)
{
	int	ret;

	if (!tree)
		ret = error_msg("syntax error", NULL, "invalid syntax", 258);
	else
	{
		ret = syntax_error_token(NULL, parser->tokens[parser->idx].type);
		free_ast(tree);
	}
	free_tokens(tokens);
	return (ret);
}

int	build_ast(t_token *tokens, t_node **root)
{
	t_parser	parser;
	t_node		*tree;

	if (!tokens || !root)
		return (1);
	if (init_parser(&parser, tokens) != 0)
		return (1);
	tree = parse_expr(&parser);
	if (!tree || parser.idx != parser.len)
		return (handle_parse_failure(tokens, tree, &parser));
	*root = tree;
	free_tokens(tokens);
	return (0);
}
