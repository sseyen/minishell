/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 15:13:39 by alisseye          #+#    #+#             */
/*   Updated: 2025/11/13 15:16:10 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

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
	return (TOKEN_ERROR);
}

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
	return (TOKEN_ERROR);
}

int	create_word_token(char *line, size_t len, t_token *token)
{
	set_token(token, (t_token_data){TOKEN_WORD, NULL, false, false});
	token->value = strndup(line, len);
	if (!token->value)
	{
		token->type = TOKEN_ERROR;
		return (1);
	}
	token->to_expand = false;
	token->quoted = false;
	return (0);
}

int	create_operator_token(char *start, size_t len, t_token *token)
{
	token->value = NULL;
	token->to_expand = false;
	token->quoted = false;
	if (len == 1)
		token->type = get_single_operator(start);
	else if (len == 2)
		token->type = get_double_operator(start);
	else
	{
		token->type = TOKEN_ERROR;
		return (1);
	}
	return (0);
}
