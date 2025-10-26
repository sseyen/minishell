/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:30:59 by alisseye          #+#    #+#             */
/*   Updated: 2025/10/26 06:07:17 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

size_t	count_word(char *line, size_t *i)
{
	size_t			j;
	t_quote_type	in_quotes;

	j = 0;
	in_quotes = NO_QUOTE;
	printf("Counting word starting at index %zu: '%s'\n", *i, &line[*i]);
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

size_t	count_operator(char *line, size_t *i)
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

void	skip_whitespaces(char *line, size_t *i)
{
	while (line[*i] && is_whitespace(line[*i]))
		(*i)++;
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
		if (count_word(&line[i], &i) != 0)
		{
			count++;
			continue ;
		}
		if (count_operator(&line[i], &i) != 0)
			count++;
	}
	return (count);
}

int	tokenize(char *line, t_token **tokens)
{
	size_t	count;

	count = count_tokens(line);
	(void)tokens;
	(void)count;
	// *tokens = malloc(sizeof(t_token) * (count + 1));
	// if (!*tokens)
	// 	return (1);
	// (*tokens)[count] = (t_token){0};
	// if (fill_tokens(line, *tokens) != 0)
	// {
	// 	free(*tokens);
	// 	*tokens = NULL;
	// 	return (1);
	// }
	// expand_tokens(*tokens);
	return (0);
}
