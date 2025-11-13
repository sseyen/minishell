/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 15:03:50 by alisseye          #+#    #+#             */
/*   Updated: 2025/11/13 15:15:09 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	skip_whitespaces(char *line, size_t *i)
{
	while (line[*i] && is_whitespace(line[*i]))
		(*i)++;
}

size_t	parse_word(char *line, size_t *i)
{
	size_t			j;
	t_quote_type	in_quotes;

	j = 0;
	in_quotes = NO_QUOTE;
	while (line[*i] \
		&& ((!is_whitespace(line[*i]) && !is_operator_char(line[*i])) \
		|| in_quotes == SINGLE_QUOTE || in_quotes == DOUBLE_QUOTE))
	{
		if (in_quotes == NO_QUOTE && line[*i] == '"')
			in_quotes = DOUBLE_QUOTE;
		else if (in_quotes == NO_QUOTE && line[*i] == '\'')
			in_quotes = SINGLE_QUOTE;
		else if (in_quotes == DOUBLE_QUOTE && line[*i] == '"')
			in_quotes = NO_QUOTE;
		else if (in_quotes == SINGLE_QUOTE && line[*i] == '\'')
			in_quotes = NO_QUOTE;
		j++;
		(*i)++;
	}
	return (j);
}

size_t	parse_operator(char *line, size_t *i)
{
	size_t	j;

	j = 0;
	while (line[*i] && is_operator_char(line[*i]))
	{
		if (j >= 1 && line[*i] != line[*i - 1])
			break ;
		j++;
		(*i)++;
	}
	return (j);
}

size_t	count_tokens(char *line)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (line[i])
	{
		skip_whitespaces(line, &i);
		if (!line[i])
			break ;
		if (parse_word(line, &i) != 0)
		{
			count++;
			continue ;
		}
		if (parse_operator(line, &i) != 0)
		{
			count++;
			continue ;
		}
		i++;
	}
	return (count);
}
