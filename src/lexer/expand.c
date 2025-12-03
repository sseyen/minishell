/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 16:04:23 by alisseye          #+#    #+#             */
/*   Updated: 2025/12/01 16:18:52 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_quote_type	handle_quote(t_quote_type quote_type, char c, size_t *i)
{
	t_quote_type	tar_quote_type;

	tar_quote_type = quote_type;
	if (c == '\'' && quote_type == NO_QUOTE)
		tar_quote_type = SINGLE_QUOTE;
	else if (c == '\'' && quote_type == SINGLE_QUOTE)
		tar_quote_type = NO_QUOTE;
	else if (c == '\"' && quote_type == NO_QUOTE)
		tar_quote_type = DOUBLE_QUOTE;
	else if (c == '\"' && quote_type == DOUBLE_QUOTE)
		tar_quote_type = NO_QUOTE;
	if (tar_quote_type != quote_type)
		(*i)++;
	return (tar_quote_type);
}

void	remove_quotes(t_token *token)
{
	size_t			i;
	size_t			j;
	t_quote_type	quote_type;

	i = 0;
	j = 0;
	quote_type = NO_QUOTE;
	while (token->value[i])
	{
		quote_type = handle_quote(quote_type, token->value[i], &i);
		token->value[j++] = token->value[i++];
	}
}

int	expand_str(t_token *token, t_shell_state *state)
{
	char	*new_value;
	char	*temp;
	size_t	i;
	size_t	from;

	i = 0;
	from = 0;
	new_value = NULL;
	temp = NULL;
	while (token->value[i])
	{
		if (token->value[i] == '$')
		{
			temp = handle_var(token->value, &from, &i, state);
			if (!temp)
				return (1);
			free(new_value);
			new_value = temp;
		}
		i++;
		if (!token->value[i])
			if (handle_eof(token->value, &new_value, from, i) != 0)
				return (1);
	}
	return (0);
}

int	expand_tokens(t_token *tokens, t_shell_state *state)
{
	size_t	i;

	i = 0;
	while (tokens[i].type != TOKEN_EOF && tokens[i].type != TOKEN_NONE)
	{
		if (tokens[i].to_expand)
			if (expand_str(&tokens[i], state) != 0)
				return (1);
		if (tokens[i].quoted)
			remove_quotes(&tokens[i]);
		i++;
	}
	return (0);
}
