/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_tokens_test.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: autogen <autogen@example.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 18:35:00 by autogen           #+#    #+#             */
/*   Updated: 2025/12/11 18:35:00 by autogen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/test.h"
#include "../../src/lexer/lexer.h"

static t_token	*alloc_tokens(char *line, size_t *count)
{
	t_token	*tokens;

	*count = count_tokens(line);
	tokens = malloc(sizeof(t_token) * (*count + 1));
	if (!tokens)
		return (NULL);
	ft_memset(tokens, 0, sizeof(t_token) * (*count + 1));
	set_token(&tokens[*count], (t_token_data){TOKEN_EOF, NULL, false, false});
	if (fill_tokens(line, tokens) != 0)
	{
		free_tokens(tokens);
		return (NULL);
	}
	return (tokens);
}

static int	check_tokens(char *line, t_token_type *expected, size_t exp_count)
{
	t_token	*tokens;
	size_t	count;
	size_t	i;
	int		ok;

	tokens = alloc_tokens(line, &count);
	if (!tokens)
	{
		print_result(0);
		return (0);
	}
	ok = 1;
	i = 0;
	while (i < exp_count)
	{
		if (tokens[i].type != expected[i])
			ok = 0;
		i++;
	}
	print_result(ok);
	free_tokens(tokens);
	return (ok);
}

int	test_fill_tokens(int argc, char **argv)
{
	int				pass;
	t_token_type	exp1[4];
	t_token_type	exp2[4];
	t_token_type	exp3[11];
	t_token_type	exp4[8];
	t_token_type	exp5[12];

	(void)argc;
	(void)argv;
	pass = 0;
	printf("\n[INIT] Testing fill_tokens...\n");
	print_test_header(0, "Words and pipe");
	exp1[0] = TOKEN_WORD;
	exp1[1] = TOKEN_WORD;
	exp1[2] = TOKEN_PIPE;
	exp1[3] = TOKEN_WORD;
	pass += check_tokens("echo hello | wc", exp1, 4);
	print_test_header(1, "Redirect operator");
	exp2[0] = TOKEN_WORD;
	exp2[1] = TOKEN_WORD;
	exp2[2] = TOKEN_REDIRECT_OUT;
	exp2[3] = TOKEN_WORD;
	pass += check_tokens("ls -l > out", exp2, 4);
	print_test_header(2, "Mixed operators");
	exp3[0] = TOKEN_WORD;
	exp3[1] = TOKEN_WORD;
	exp3[2] = TOKEN_REDIRECT_APPEND;
	exp3[3] = TOKEN_WORD;
	exp3[4] = TOKEN_PIPE;
	exp3[5] = TOKEN_OR;
	exp3[6] = TOKEN_AND;
	exp3[7] = TOKEN_WORD;
	exp3[8] = TOKEN_LPAREN;
	exp3[9] = TOKEN_RPAREN;
	exp3[10] = TOKEN_WORD;
	pass += check_tokens("cmd1 cmd2 >> file | || && cmd3 ( ) cmd4", exp3, 11);
	print_test_header(3, "Sequential redirects without operands");
	exp4[0] = TOKEN_WORD;
	exp4[1] = TOKEN_WORD;
	exp4[2] = TOKEN_REDIRECT_APPEND;
	exp4[3] = TOKEN_REDIRECT_OUT;
	exp4[4] = TOKEN_PIPE;
	exp4[5] = TOKEN_AND;
	exp4[6] = TOKEN_OR;
	exp4[7] = TOKEN_WORD;
	pass += check_tokens("word word>>> | && || word", exp4, 8);
	print_test_header(4, "Complex command with subshells and redirection");
	exp5[0] = TOKEN_LPAREN;
	exp5[1] = TOKEN_WORD;
	exp5[2] = TOKEN_WORD;
	exp5[3] = TOKEN_AND;
	exp5[4] = TOKEN_WORD;
	exp5[5] = TOKEN_WORD;
	exp5[6] = TOKEN_AND;
	exp5[7] = TOKEN_WORD;
	exp5[8] = TOKEN_WORD;
	exp5[9] = TOKEN_RPAREN;
	exp5[10] = TOKEN_REDIRECT_OUT;
	exp5[11] = TOKEN_WORD;
	pass += check_tokens("(ls -l && echo \"---\" && cat file) > output.txt", exp5, 12);
	print_summary(5, pass);
	return (0);
}
