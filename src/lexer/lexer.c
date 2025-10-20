/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:30:59 by alisseye          #+#    #+#             */
/*   Updated: 2025/10/20 21:58:35 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


size_t	count_tokens(char *line)
{
	size_t	count;
	size_t	i;
	int		quote;

	count = 0;
	i = 0;
	quote = 0;
	while (line[i])
	{
		while (line[i] && ft_isspace(line[i]))
			i++;
		while (line[i] \
			&& !ft_isspace(line[i]) && !is_special_char(line[i], quote)) // Парсинг слова
		{
			i++;
			count++;
			continue ;
		}
		while (line[i] && is_special_char(line[i], quote)) // Парсинг оператора
		{
			i++;
			count++;
			continue ;
		}
	}
	return (count);
}

int	tokenize(char *line, t_token **tokens)
{
	size_t	count;

	count = count_tokens(line);
	*tokens = malloc(sizeof(t_token) * (count + 1));
	if (!*tokens)
		return (1);
	(*tokens)[count] = (t_token){0};
	if (fill_tokens(line, *tokens) != 0)
	{
		free(*tokens);
		*tokens = NULL;
		return (1);
	}
	expand_tokens(*tokens);
	return (0);
}
