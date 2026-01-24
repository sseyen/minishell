/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:30:59 by alisseye          #+#    #+#             */
/*   Updated: 2025/12/14 19:55:07 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	free_tokens(t_token *tokens)
{
	size_t	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i].type != TOKEN_NONE && tokens[i].type != TOKEN_EOF)
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
	int		ret_code;

	i = 0;
	tok_index = 0;
	while (line[i])
	{
		skip_whitespaces(line, &i);
		if (!line[i])
			break ;
		ret_code = create_token(line, &i, &tokens[tok_index]);
		if (ret_code != 0)
			return (ret_code);
		tok_index++;
	}
	return (0);
}

int	process_tokens(char *line, t_token *tokens, t_shell_state *state)
{
	int	ret_code;

	ret_code = fill_tokens(line, tokens);
	if (ret_code != 0)
		return (ret_code);
	ret_code = validate_tokens(tokens);
	if (ret_code != 0)
		return (258);
	ret_code = expand_tokens(tokens, state);
	if (ret_code != 0)
		return (1);
	return (0);
}

int	tokenize(char *line, t_token **tokens, t_shell_state *state)
{
	size_t			count;
	int				ret_code;

	ret_code = 0;
	count = count_tokens(line);
	if (count == SIZE_MAX)
		return (error_msg("syntax error", NULL, "unclosed quotes", 258));
	*tokens = malloc(sizeof(t_token) * (count + 1));
	if (!*tokens)
		return (1);
	ft_memset(*tokens, 0, sizeof(t_token) * (count + 1));
	set_token(&(*tokens)[count], (t_token_data){TOKEN_EOF, NULL, false, false});
	ret_code = process_tokens(line, *tokens, state);
	if (ret_code != 0)
	{
		free_tokens(*tokens);
		*tokens = NULL;
		return (ret_code);
	}
	return (0);
}
