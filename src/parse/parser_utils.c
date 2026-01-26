/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 18:55:00 by alisseye          #+#    #+#             */
/*   Updated: 2026/01/26 01:52:07 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * @brief
 * Counts number of tokens in array (excluding EOF/NONE).
 */
static size_t	count_len(t_token *tokens)
{
	size_t	len;

	len = 0;
	while (tokens[len].type != TOKEN_EOF && tokens[len].type != TOKEN_NONE)
		len++;
	return (len);
}

/**
 * @brief
 * Initializes parser state with token array.
 */
int	init_parser(t_parser *parser, t_token *tokens)
{
	if (!parser || !tokens)
		return (1);
	parser->tokens = tokens;
	parser->idx = 0;
	parser->len = count_len(tokens);
	return (0);
}

/**
 * @brief
 * Allocates and initializes a new AST node.
 */
t_node	*new_node(t_node_type type)
{
	t_node	*node;

	node = ft_calloc(1, sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = type;
	return (node);
}
