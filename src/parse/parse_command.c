/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 18:55:20 by alisseye          #+#    #+#             */
/*   Updated: 2025/12/14 18:52:13 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * @brief Map token type to redirect enum.
 *
 * @param tok Token type from lexer.
 *
 * @return Corresponding t_redirect_type.
 */
static t_redirect_type	redir_type(t_token_type tok)
{
	if (tok == TOKEN_REDIRECT_IN)
		return (REDIRECT_IN);
	if (tok == TOKEN_REDIRECT_OUT)
		return (REDIRECT_OUT);
	if (tok == TOKEN_REDIRECT_APPEND)
		return (REDIRECT_APPEND);
	return (REDIRECT_HEREDOC);
}

/**
 * @brief Append a redirect target to the command node.
 *
 * @param p Parser state (idx advanced on success).
 * @param node Command node being built.
 * @param ri Redirect write index.
 *
 * @return 0 on success, 1 on error.
 */
static int	add_redirect(struct s_parser *p, t_node *node, size_t *ri)
{
	t_token	*target;

	if (p->idx + 1 >= p->len)
		return (1);
	target = &p->tokens[p->idx + 1];
	if (target->type != TOKEN_WORD || !target->value)
		return (1);
	node->redirects[*ri].type = redir_type(p->tokens[p->idx].type);
	node->redirects[*ri].target = ft_strdup(target->value);
	node->redirects[*ri].heredoc_fd = -1;
	node->redirects[*ri].heredoc_quoted = target->quoted;
	if (!node->redirects[*ri].target)
		return (1);
	(*ri)++;
	node->redirects_count = *ri;
	p->idx += 2;
	return (0);
}

/**
 * @brief Allocate a NODE_CMD node and its arrays.
 *
 * @param p Parser state (used for capacity estimates).
 * @param node Out pointer to the allocated node.
 * @param c Command parse context to seed indices/capacity.
 *
 * @return 0 on success, 1 on allocation failure.
 */
static int	alloc_command(struct s_parser *p, t_node **node,
				t_cmd_ctx *c)
{
	*node = new_node(NODE_CMD);
	if (!*node)
		return (1);
	c->argv_cap = p->len - p->idx + 1;
	c->redir_cap = c->argv_cap;
	c->ai = 0;
	c->ri = 0;
	(*node)->argv = ft_calloc(c->argv_cap, sizeof(char *));
	(*node)->redirects = ft_calloc(c->redir_cap, sizeof(t_redirect));
	if (!(*node)->argv || !(*node)->redirects)
		return (free_ast(*node), 1);
	return (0);
}

/**
 * @brief Consume one token into argv or redirects.
 *
 * @param p Parser state.
 * @param node Command node under construction.
 * @param c Command parse context tracking indices.
 *
 * @return 0 on success, 1 on error, 2 when no more command tokens.
 */
static int	process_token(struct s_parser *p, t_node *node,
				t_cmd_ctx *c)
{
	t_token	*tok;

	tok = &p->tokens[p->idx];
	if (tok->type == TOKEN_WORD)
	{
		node->argv[c->ai] = ft_strdup(tok->value);
		if (!node->argv[c->ai])
			return (1);
		node->argc++;
		c->ai++;
		p->idx++;
		return (0);
	}
	if (tok->type == TOKEN_REDIRECT_IN || tok->type == TOKEN_REDIRECT_OUT
		|| tok->type == TOKEN_REDIRECT_APPEND
		|| tok->type == TOKEN_REDIRECT_HEREDOC)
	{
		if (add_redirect(p, node, &c->ri) != 0)
			return (1);
		return (0);
	}
	return (2);
}

/**
 * @brief Parse a simple command with arguments and redirects.
 *
 * @param p Parser state.
 *
 * @return NODE_CMD tree node or NULL on syntax/alloc failure.
 */
t_node	*parse_command(t_parser *p)
{
	t_node		*node;
	t_cmd_ctx	ctx;

	if (!p || p->idx >= p->len)
		return (NULL);
	if (alloc_command(p, &node, &ctx) != 0)
		return (NULL);
	while (p->idx < p->len)
	{
		ctx.status = process_token(p, node, &ctx);
		if (ctx.status == 1)
			return (free_ast(node), NULL);
		if (ctx.status == 2)
			break ;
	}
	if (node->argc == 0 && node->redirects_count == 0)
		return (free_ast(node), NULL);
	return (node);
}
