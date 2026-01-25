/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 18:08:27 by alisseye          #+#    #+#             */
/*   Updated: 2026/01/25 00:20:32 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include "minishell.h"

typedef struct s_cmd_ctx
{
	size_t		argv_cap;
	size_t		redir_cap;
	size_t		ai;
	size_t		ri;
	int			status;
}				t_cmd_ctx;

typedef struct s_parser
{
	t_token		*tokens;
	size_t		idx;
	size_t		len;
}				t_parser;

int				init_parser(t_parser *parser, t_token *tokens);
t_node			*parse_command(t_parser *parser);
t_node			*parse_primary(t_parser *parser);
t_node			*parse_expr(t_parser *parser);
t_node			*parse_pipe(t_parser *parser);
t_node			*parse_and(t_parser *parser);
t_node			*new_node(t_node_type type);
bool			is_redir(t_token_type type);
t_redirect_type	redir_type(t_token_type tok);
int				fill_redirect(t_parser *p, t_redirect *dst);
int				parse_suffix_redirects(t_parser *p, t_node *node);

#endif