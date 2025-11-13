/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:30:59 by alisseye          #+#    #+#             */
/*   Updated: 2025/11/13 15:15:24 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	free_tokens(t_token *tokens)
{
	size_t	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i].type != TOKEN_ERROR && tokens[i].type != TOKEN_EOF)
	{
		if (tokens[i].value)
			free(tokens[i].value);
		i++;
	}
	free(tokens);
}

int	fill_tokens(char *line, t_token *tokens)
{
	size_t	i;
	size_t	tok_index;

	i = 0;
	tok_index = 0;
	while (line[i])
	{
		skip_whitespaces(line, &i);
		if (!line[i])
			break ;
		if (create_token(line, &i, &tokens[tok_index]) != 0)
		{
			free_tokens(tokens);
			return (1);
		}
		tok_index++;
	}
	return (0);
}

int	tokenize(char *line, t_token **tokens)
{
	size_t			count;

	count = count_tokens(line);
	*tokens = malloc(sizeof(t_token) * (count + 1));
	if (!*tokens)
		return (1);
	set_token(&(*tokens)[count], (t_token_data){TOKEN_EOF, NULL, false, false});
	if (fill_tokens(line, *tokens) != 0)
	{
		free(*tokens);
		*tokens = NULL;
		return (1);
	}
	// expand_tokens(*tokens);
	return (0);
}
