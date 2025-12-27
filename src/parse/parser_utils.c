/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 18:55:00 by alisseye          #+#    #+#             */
/*   Updated: 2025/12/14 18:27:37 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * @brief Count tokens until TOKEN_EOF or TOKEN_NONE sentinel.
 *
 * @param tokens Token array.
 *
 * @return Number of usable tokens.
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
 * @brief Initialize parser state from a token array.
 *
 * @param parser Parser state to fill.
 * @param tokens Token array.
 *
 * @return 0 on success, 1 when arguments are NULL.
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
 * @brief Allocate and zero a parser AST node of the given type.
 *
 * @param type AST node type.
 *
 * @return Node pointer or NULL on allocation failure.
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
