/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 23:32:21 by alisseye          #+#    #+#             */
/*   Updated: 2026/01/26 19:52:38 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

/**
 * @brief
 * Checks for errors at end of token stream.
 * Validates no trailing operators and balanced parens.
 */
static int	finalize_validation(t_token_type prev, int paren)
{
	if (is_binary_operator(prev) || is_redirect_operator(prev))
		return (syntax_error_token(NULL, TOKEN_NONE));
	if (paren != 0)
		return (error_msg("syntax error", NULL, "unclosed parenthesis",
				SYNTAX_ERROR));
	return (0);
}

/**
 * @brief
 * Checks if two consecutive token types form an invalid sequence.
 */
bool	is_invalid_sequence(t_token_type prev, t_token_type curr)
{
	if (prev == TOKEN_NONE && is_binary_operator(curr))
		return (true);
	if (is_binary_operator(prev) && is_binary_operator(curr))
		return (true);
	if (is_redir_error(prev, curr))
		return (true);
	if (is_paren_error(prev, curr))
		return (true);
	return (false);
}

/**
 * @brief
 * Validates token syntax (operator sequence, parentheses balance).
 *
 * @return 0 on success, SYNTAX_ERROR on syntax error.
 */
int	validate_tokens(t_token *tokens)
{
	size_t			i;
	t_token_type	prev;
	int				paren;

	i = 0;
	paren = 0;
	prev = TOKEN_NONE;
	while (tokens[i].type != TOKEN_NONE && tokens[i].type != TOKEN_EOF)
	{
		if (is_invalid_sequence(prev, tokens[i].type))
			return (syntax_error_token(NULL, tokens[i].type));
		if (tokens[i].type == TOKEN_LPAREN)
			paren++;
		else if (tokens[i].type == TOKEN_RPAREN)
			paren--;
		if (paren < 0)
			return (syntax_error_token(NULL, TOKEN_RPAREN));
		prev = tokens[i].type;
		i++;
	}
	return (finalize_validation(prev, paren));
}
