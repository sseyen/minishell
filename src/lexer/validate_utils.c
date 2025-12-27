/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 23:29:43 by alisseye          #+#    #+#             */
/*   Updated: 2025/12/11 13:21:21 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

bool	is_binary_operator(t_token_type type)
{
	return (type == TOKEN_PIPE || type == TOKEN_OR || type == TOKEN_AND);
}

bool	is_redirect_operator(t_token_type type)
{
	return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT || \
		type == TOKEN_REDIRECT_APPEND || type == TOKEN_REDIRECT_HEREDOC);
}

bool	is_redir_error(t_token_type prev, t_token_type curr)
{
	if (!is_redirect_operator(prev))
		return (false);
	if (is_binary_operator(curr) || is_redirect_operator(curr))
		return (true);
	if (curr == TOKEN_RPAREN || curr == TOKEN_EOF || curr == TOKEN_LPAREN)
		return (true);
	return (false);
}

bool	is_paren_error(t_token_type prev, t_token_type curr)
{
	if (curr == TOKEN_RPAREN && (is_binary_operator(prev) || \
		prev == TOKEN_LPAREN))
		return (true);
	if (curr == TOKEN_LPAREN && (prev == TOKEN_WORD || prev == TOKEN_RPAREN))
		return (true);
	return (false);
}
