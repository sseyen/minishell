/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 15:03:50 by alisseye          #+#    #+#             */
/*   Updated: 2026/01/26 01:52:07 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

/**
 * @brief
 * Advances index past whitespace characters.
 */
void	skip_whitespaces(char *line, size_t *i)
{
	while (line[*i] && is_whitespace(line[*i]))
		(*i)++;
}

/**
 * @brief
 * Updates quote state machine based on current character.
 *
 * @return New quote state.
 */
t_quote_type	update_quote_state(t_quote_type state, char c)
{
	if (c == '\'' && state == NO_QUOTE)
		return (SINGLE_QUOTE);
	if (c == '\'' && state == SINGLE_QUOTE)
		return (NO_QUOTE);
	if (c == '\"' && state == NO_QUOTE)
		return (DOUBLE_QUOTE);
	if (c == '\"' && state == DOUBLE_QUOTE)
		return (NO_QUOTE);
	return (state);
}

/**
 * @brief
 * Parses a word token (handles quotes).
 *
 * @return Word length, SIZE_MAX if unclosed quotes.
 */
size_t	parse_word(char *line, size_t *i, size_t *count)
{
	size_t			j;
	t_quote_type	in_quotes;

	j = 0;
	in_quotes = NO_QUOTE;
	while (line[*i] && ((!is_whitespace(line[*i])
				&& !is_operator_char(line[*i])) || in_quotes == SINGLE_QUOTE
			|| in_quotes == DOUBLE_QUOTE))
	{
		in_quotes = update_quote_state(in_quotes, line[*i]);
		j++;
		(*i)++;
	}
	if (in_quotes != NO_QUOTE)
		return (SIZE_MAX);
	if (count && j != 0)
		(*count)++;
	return (j);
}

/**
 * @brief
 * Parses an operator token (|, ||, &&, <, >, etc.).
 *
 * @return Operator length.
 */
size_t	parse_operator(char *line, size_t *i, size_t *count)
{
	size_t	j;

	j = 0;
	while (line[*i] && is_operator_char(line[*i]))
	{
		if (j >= 2)
			break ;
		if (j >= 1 && line[*i] != line[*i - 1])
			break ;
		j++;
		(*i)++;
		if (line[*i - 1] == '(' || line[*i - 1] == ')')
			break ;
	}
	if (count && j != 0)
		(*count)++;
	return (j);
}

/**
 * @brief
 * Counts total tokens in a command line for allocation.
 *
 * @return Token count, SIZE_MAX on unclosed quotes.
 */
size_t	count_tokens(char *line)
{
	size_t	count;
	size_t	i;
	size_t	token_length;

	count = 0;
	i = 0;
	while (line[i])
	{
		skip_whitespaces(line, &i);
		if (!line[i])
			break ;
		token_length = parse_word(line, &i, &count);
		if (token_length != 0)
		{
			if (token_length == SIZE_MAX)
				return (SIZE_MAX);
			continue ;
		}
		token_length = parse_operator(line, &i, &count);
		if (token_length != 0)
			continue ;
	}
	return (count);
}
