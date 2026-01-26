/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 12:00:00 by alisseye          #+#    #+#             */
/*   Updated: 2026/01/26 01:52:07 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * @brief
 * Checks if token type is a redirect operator.
 */
bool	is_redir(t_token_type type)
{
	return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_REDIRECT_APPEND || type == TOKEN_REDIRECT_HEREDOC);
}

/**
 * @brief
 * Converts token type to redirect type enum.
 */
t_redirect_type	redir_type(t_token_type tok)
{
	if (tok == TOKEN_REDIRECT_IN)
		return (REDIRECT_IN);
	if (tok == TOKEN_REDIRECT_OUT)
		return (REDIRECT_OUT);
	if (tok == TOKEN_REDIRECT_APPEND)
		return (REDIRECT_APPEND);
	return (REDIRECT_HEREDOC);
}

/**
 * @brief
 * Fills a redirect struct from current parser position.
 * Advances parser index by 2 (operator + target).
 */
int	fill_redirect(t_parser *p, t_redirect *dst)
{
	t_token	*target;

	if (p->idx + 1 >= p->len)
		return (1);
	target = &p->tokens[p->idx + 1];
	if (target->type != TOKEN_WORD || !target->value)
		return (1);
	dst->type = redir_type(p->tokens[p->idx].type);
	dst->target = ft_strdup(target->value);
	dst->heredoc_fd = -1;
	dst->heredoc_quoted = target->quoted;
	if (!dst->target)
		return (1);
	p->idx += 2;
	return (0);
}
