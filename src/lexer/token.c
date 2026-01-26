/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 15:11:12 by alisseye          #+#    #+#             */
/*   Updated: 2026/01/26 01:52:07 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

/**
 * @brief
 * Initializes a token with given data.
 */
void	set_token(t_token *token, t_token_data data)
{
	token->type = data.type;
	token->value = data.value;
	token->to_expand = data.to_expand;
	token->quoted = data.quoted;
}

/**
 * @brief
 * Creates a WORD token from a string.
 *
 * @return 0 on success, 1 on malloc failure.
 */
int	create_word_token(char *line, size_t len, t_token *token)
{
	token->type = TOKEN_WORD;
	token->value = ft_strndup(line, len);
	if (!token->value)
		return (1);
	token->quoted = is_quoted(token->value);
	token->to_expand = to_expand(token->value);
	return (0);
}

/**
 * @brief
 * Creates an operator token (|, ||, &&, redirects, parens).
 *
 * @return 0 on success, SYNTAX_ERROR on unknown operator.
 */
int	create_operator_token(char *start, size_t len, t_token *token)
{
	char	tok[8];

	if (len == 1)
		token->type = get_single_operator(start);
	else if (len == 2)
		token->type = get_double_operator(start);
	if (token->type == TOKEN_NONE)
	{
		if (len >= sizeof(tok))
			len = sizeof(tok) - 1;
		ft_bzero(tok, sizeof(tok));
		ft_memcpy(tok, start, len);
		tok[len] = '\0';
		return (syntax_error_token(tok, TOKEN_NONE));
	}
	return (0);
}

/**
 * @brief
 * Creates a token from current position in line.
 * Dispatches to word or operator token creation.
 */
int	create_token(char *line, size_t *i, t_token *token)
{
	size_t	start;
	size_t	len;
	char	tok[2];

	start = *i;
	len = parse_word(line, i, NULL);
	if (len > 0)
		return (create_word_token(&line[start], len, token));
	start = *i;
	len = parse_operator(line, i, NULL);
	if (len > 0)
		return (create_operator_token(&line[start], len, token));
	ft_bzero(tok, sizeof(tok));
	tok[0] = line[*i];
	tok[1] = '\0';
	return (syntax_error_token(tok, TOKEN_NONE));
}
