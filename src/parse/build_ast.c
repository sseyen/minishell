/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 17:54:55 by alisseye          #+#    #+#             */
/*   Updated: 2025/12/14 18:26:27 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * @brief Entry point to parse the token stream into an AST.
 *
 * @param tokens Token array terminated by TOKEN_EOF.
 * @param root Output parameter receiving the built tree on success.
 *
 * @return 0 on success, 1 on allocation or parse failure.
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
	{
		if (tree)
			free_ast(tree);
		free_tokens(tokens);
		return (1);
	}
	*root = tree;
	free_tokens(tokens);
	return (0);
}
