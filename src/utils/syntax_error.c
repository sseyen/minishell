/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 12:20:00 by alisseye          #+#    #+#             */
/*   Updated: 2026/01/26 01:52:06 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief
 * Returns string representation of a token type for error messages.
 */
static const char	*token_repr(t_token_type type)
{
	if (type == TOKEN_PIPE)
		return ("|");
	if (type == TOKEN_OR)
		return ("||");
	if (type == TOKEN_AND)
		return ("&&");
	if (type == TOKEN_REDIRECT_IN)
		return ("<");
	if (type == TOKEN_REDIRECT_OUT)
		return (">");
	if (type == TOKEN_REDIRECT_APPEND)
		return (">>");
	if (type == TOKEN_REDIRECT_HEREDOC)
		return ("<<");
	if (type == TOKEN_LPAREN)
		return ("(");
	if (type == TOKEN_RPAREN)
		return (")");
	return ("newline");
}

/**
 * @brief
 * Prints syntax error message for unexpected token.
 *
 * @param tok Token string (NULL to use type representation).
 * @param type Token type.
 * @return 258 (syntax error exit code).
 */
int	syntax_error_token(const char *tok, t_token_type type)
{
	char		msg[64];
	const char	*repr;

	repr = tok;
	if (repr == NULL)
		repr = token_repr(type);
	ft_strlcpy(msg, "near unexpected token `", sizeof(msg));
	ft_strlcat(msg, repr, sizeof(msg));
	ft_strlcat(msg, "'", sizeof(msg));
	return (error_msg("syntax error", NULL, msg, 258));
}
