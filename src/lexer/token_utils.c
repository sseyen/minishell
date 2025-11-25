/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 15:13:39 by alisseye          #+#    #+#             */
/*   Updated: 2025/11/23 16:27:35 by alisseye         ###   ########.fr       */
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
	return (TOKEN_NONE);
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
	return (TOKEN_NONE);
}

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

bool	to_expand(char *str)
{
	size_t			i;
	t_quote_type	quoted;

	i = 0;
	quoted = NO_QUOTE;
	while (str[i])
	{
		if (str[i] == '\'' && quoted == NO_QUOTE)
			quoted = SINGLE_QUOTE;
		else if (str[i] == '\"' && quoted == NO_QUOTE)
			quoted = DOUBLE_QUOTE;
		else if (str[i] == '\'' && quoted == SINGLE_QUOTE)
			quoted = NO_QUOTE;
		else if (str[i] == '\"' && quoted == DOUBLE_QUOTE)
			quoted = NO_QUOTE;
		if (str[i] == '$' && quoted != SINGLE_QUOTE)
			return (true);
		i++;
	}
	return (false);
}
