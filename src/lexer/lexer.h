/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 04:23:37 by alisseye          #+#    #+#             */
/*   Updated: 2025/11/13 15:17:05 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"

typedef enum e_quote_type
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
}		t_quote_type;

typedef struct s_token_data
{
	t_token_type	type;
	char			*value;
	bool			to_expand;
	bool			quoted;
}				t_token_data;

// Lexer
int				tokenize(char *line, t_token **tokens);
void			free_tokens(t_token *tokens);
int				fill_tokens(char *line, t_token *tokens);

// Token utils
void			set_token(t_token *token, t_token_data data);
int				create_token(char *line, size_t *i, t_token *token);
t_token_type	get_single_operator(char *start);
t_token_type	get_double_operator(char *start);
int				create_word_token(char *line, size_t len, t_token *token);
int				create_operator_token(char *start, size_t len, t_token *token);

// Parser utils
void			skip_whitespaces(char *line, size_t *i);
size_t			parse_word(char *line, size_t *i);
size_t			parse_operator(char *line, size_t *i);
size_t			count_tokens(char *line);

#endif