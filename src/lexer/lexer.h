/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 04:23:37 by alisseye          #+#    #+#             */
/*   Updated: 2026/01/25 00:18:53 by danslav1e        ###   ########.fr       */
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
}					t_quote_type;

typedef struct s_token_data
{
	t_token_type	type;
	char			*value;
	bool			to_expand;
	bool			quoted;
}					t_token_data;

// Lexer
int					tokenize(char *line, t_token **tokens,
						t_shell_state *state);
void				free_tokens(t_token *tokens);
int					fill_tokens(char *line, t_token *tokens);
int					expand_tokens(t_token *tokens, t_shell_state *state);

// Parse
t_quote_type		update_quote_state(t_quote_type state, char c);

// Validate
int					validate_tokens(t_token *tokens);
bool				is_redir_error(t_token_type prev, t_token_type curr);
bool				is_paren_error(t_token_type prev, t_token_type curr);
bool				is_binary_operator(t_token_type type);
bool				is_redirect_operator(t_token_type type);

// Token utils
void				set_token(t_token *token, t_token_data data);
int					create_token(char *line, size_t *i, t_token *token);
t_token_type		get_single_operator(char *start);
t_token_type		get_double_operator(char *start);
int					create_word_token(char *line, size_t len, t_token *token);
int					create_operator_token(char *start, size_t len,
						t_token *token);

// Parse utils
void				skip_whitespaces(char *line, size_t *i);
size_t				parse_word(char *line, size_t *i, size_t *count);
size_t				parse_operator(char *line, size_t *i, size_t *count);
size_t				count_tokens(char *line);
bool				is_quoted(char *str);
bool				to_expand(char *str);

// Expand utils
char				*join_strings(char *s1, char *s2, size_t from, size_t to);
char				*parse_var(char *str, t_shell_state *state, size_t *index);
int					handle_eof(char *token_value, char **new_value, size_t from,
						size_t i);

#endif