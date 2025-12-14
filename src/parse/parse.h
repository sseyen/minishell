/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 18:08:27 by alisseye          #+#    #+#             */
/*   Updated: 2025/12/14 18:53:02 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include "minishell.h"

typedef struct s_cmd_ctx
{
	size_t	argv_cap;
	size_t	redir_cap;
	size_t	ai;
	size_t	ri;
	int		status;
}				t_cmd_ctx;

typedef struct s_parser
{
	t_token		*tokens;
	size_t		idx;
	size_t		len;
}				t_parser;

int				init_parser(t_parser *parser, t_token *tokens);
t_node			*parse_command(t_parser *parser);
t_node			*parse_expr(t_parser *parser);
t_node			*new_node(t_node_type type);

#endif