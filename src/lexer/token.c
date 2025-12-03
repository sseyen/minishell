/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 15:11:12 by alisseye          #+#    #+#             */
/*   Updated: 2025/11/23 16:30:00 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	set_token(t_token *token, t_token_data data)
{
	token->type = data.type;
	token->value = data.value;
	token->to_expand = data.to_expand;
	token->quoted = data.quoted;
}

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

int	create_operator_token(char *start, size_t len, t_token *token)
{
	if (len == 1)
		token->type = get_single_operator(start);
	else if (len == 2)
		token->type = get_double_operator(start);
	if (token->type == TOKEN_NONE)
		return (1);
	return (0);
}

int	create_token(char *line, size_t *i, t_token *token)
{
	size_t	start;
	size_t	len;

	start = *i;
	len = parse_word(line, i);
	if (len > 0)
		return (create_word_token(&line[start], len, token));
	start = *i;
	len = parse_operator(line, i);
	if (len > 0)
		return (create_operator_token(&line[start], len, token));
	return (1);
}
