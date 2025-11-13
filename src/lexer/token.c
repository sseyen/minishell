/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 15:11:12 by alisseye          #+#    #+#             */
/*   Updated: 2025/11/13 15:14:14 by alisseye         ###   ########.fr       */
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
	set_token(token, (t_token_data){TOKEN_ERROR, NULL, false, false});
	return (1);
}
