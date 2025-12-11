/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 23:32:21 by alisseye          #+#    #+#             */
/*   Updated: 2025/12/11 13:21:18 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

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
			return (258);
		if (tokens[i].type == TOKEN_LPAREN)
			paren++;
		else if (tokens[i].type == TOKEN_RPAREN)
			paren--;
		if (paren < 0)
			return (258);
		prev = tokens[i].type;
		i++;
	}
	if (is_binary_operator(prev) || is_redirect_operator(prev))
		return (258);
	if (paren != 0)
		return (258);
	return (0);
}
