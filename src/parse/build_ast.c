/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 17:54:55 by alisseye          #+#    #+#             */
/*   Updated: 2026/01/26 19:52:38 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * @brief
 * Handles parse failure: prints error, frees tree and tokens.
 */
static int	handle_parse_failure(t_token *tokens, t_node *tree,
	t_parser *parser)
{
	int	ret;

	if (!tree)
		ret = error_msg("syntax error", NULL, "invalid syntax",
				SYNTAX_ERROR);
	else
	{
		ret = syntax_error_token(NULL, parser->tokens[parser->idx].type);
		free_ast(tree);
	}
	free_tokens(tokens);
	return (ret);
}

/**
 * @brief
 * Main entry point for AST construction.
 * Parses tokens into an executable AST tree.
 *
 * @param tokens Array of tokens from lexer.
 * @param root Output pointer for AST root.
 * @return 0 on success, error code otherwise.
 */
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
