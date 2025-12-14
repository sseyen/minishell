/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 23:32:21 by alisseye          #+#    #+#             */
/*   Updated: 2025/12/14 19:55:07 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

static int	syntax_error_token(const char *tok)
{
	char	msg[64];

	ft_strlcpy(msg, "near unexpected token `", sizeof(msg));
	ft_strlcat(msg, tok, sizeof(msg));
	ft_strlcat(msg, "'", sizeof(msg));
	return (error_msg("syntax error", NULL, msg, 258));
}

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

static int	finalize_validation(t_token_type prev, int paren)
{
	if (is_binary_operator(prev) || is_redirect_operator(prev))
		return (syntax_error_token("newline"));
	if (paren != 0)
		return (error_msg("syntax error", NULL, "unclosed parenthesis", 258));
	return (0);
}

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
			return (syntax_error_token(token_repr(tokens[i].type)));
		if (tokens[i].type == TOKEN_LPAREN)
			paren++;
		else if (tokens[i].type == TOKEN_RPAREN)
			paren--;
		if (paren < 0)
			return (syntax_error_token(token_repr(TOKEN_RPAREN)));
		prev = tokens[i].type;
		i++;
	}
	return (finalize_validation(prev, paren));
}
