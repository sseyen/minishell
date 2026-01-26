/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 16:04:23 by alisseye          #+#    #+#             */
/*   Updated: 2026/01/26 01:52:07 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

/**
 * @brief
 * Removes quote characters from a token value in-place.
 * Tracks quote state to handle nested quotes correctly.
 */
void	remove_quotes(t_token *token)
{
	size_t			r;
	size_t			w;
	t_quote_type	quote_type;
	t_quote_type	new_quote;

	r = 0;
	w = 0;
	quote_type = NO_QUOTE;
	while (token->value[r])
	{
		new_quote = update_quote_state(quote_type, token->value[r]);
		if (new_quote != quote_type)
		{
			quote_type = new_quote;
			r++;
			continue ;
		}
		quote_type = new_quote;
		token->value[w++] = token->value[r++];
	}
	token->value[w] = '\0';
}

/**
 * @brief
 * Processes a $ variable expansion.
 * Joins text before $ with expanded variable value.
 */
static int	process_dollar(t_token *token, t_shell_state *state,
							t_expand_ctx *ctx)
{
	char	*temp;
	char	*var_value;
	size_t	start;

	start = ctx->index;
	ctx->index++;
	temp = join_strings(*(ctx->new_value), token->value, ctx->from, start);
	if (!temp)
		return (1);
	var_value = parse_var(&token->value[ctx->index], state, &ctx->index);
	if (!var_value)
	{
		free(temp);
		return (1);
	}
	ctx->from = ctx->index;
	temp = join_strings(temp, var_value, 0, ft_strlen(var_value));
	free(var_value);
	if (!temp)
		return (1);
	*(ctx->new_value) = temp;
	return (0);
}

/**
 * @brief
 * Walks through token value expanding all $ variables.
 * Respects single quotes (no expansion inside).
 */
static int	walk_value(t_token *token, t_shell_state *state, char **new_value)
{
	t_expand_ctx	ctx;
	t_quote_type	quote;

	ctx.index = 0;
	ctx.from = 0;
	ctx.new_value = new_value;
	quote = NO_QUOTE;
	while (token->value[ctx.index])
	{
		quote = update_quote_state(quote, token->value[ctx.index]);
		if (token->value[ctx.index] == '$' && quote != SINGLE_QUOTE)
		{
			if (process_dollar(token, state, &ctx) != 0)
				return (1);
			continue ;
		}
		ctx.index++;
	}
	if (handle_eof(token->value, new_value, ctx.from, ctx.index) != 0)
		return (1);
	return (0);
}

/**
 * @brief
 * Expands all variables in a single token.
 *
 * @return 0 on success, 1 on malloc failure.
 */
int	expand_str(t_token *token, t_shell_state *state)
{
	char	*new_value;

	new_value = NULL;
	if (walk_value(token, state, &new_value) != 0)
		return (1);
	if (new_value)
	{
		free(token->value);
		token->value = new_value;
	}
	return (0);
}

/**
 * @brief
 * Expands variables and removes quotes from all tokens.
 *
 * @return 0 on success, 1 on error.
 */
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
