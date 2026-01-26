/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 15:13:39 by alisseye          #+#    #+#             */
/*   Updated: 2026/01/26 01:52:07 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

/**
 * @brief
 * Returns token type for single-char operators.
 */
t_token_type	get_single_operator(char *start)
{
	if (start[0] == '>')
		return (TOKEN_REDIRECT_OUT);
	else if (start[0] == '<')
		return (TOKEN_REDIRECT_IN);
	else if (start[0] == '|')
		return (TOKEN_PIPE);
	else if (start[0] == '(')
		return (TOKEN_LPAREN);
	else if (start[0] == ')')
		return (TOKEN_RPAREN);
	return (TOKEN_NONE);
}

/**
 * @brief
 * Returns token type for double-char operators (||, &&, >>, <<).
 */
t_token_type	get_double_operator(char *start)
{
	if (start[0] == '>' && start[1] == '>')
		return (TOKEN_REDIRECT_APPEND);
	else if (start[0] == '<' && start[1] == '<')
		return (TOKEN_REDIRECT_HEREDOC);
	else if (start[0] == '|' && start[1] == '|')
		return (TOKEN_OR);
	else if (start[0] == '&' && start[1] == '&')
		return (TOKEN_AND);
	return (TOKEN_NONE);
}

/**
 * @brief
 * Checks if a string contains quote characters.
 */
bool	is_quoted(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			return (true);
		i++;
	}
	return (false);
}

/**
 * @brief
 * Checks if string contains $ that needs expansion.
 * Ignores $ inside single quotes.
 */
bool	to_expand(char *str)
{
	size_t			i;
	t_quote_type	quoted;

	i = 0;
	quoted = NO_QUOTE;
	while (str[i])
	{
		quoted = update_quote_state(quoted, str[i]);
		if (str[i] == '$' && quoted != SINGLE_QUOTE)
			return (true);
		i++;
	}
	return (false);
}
