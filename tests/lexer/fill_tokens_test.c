/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_tokens_test.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 14:07:05 by alisseye          #+#    #+#             */
/*   Updated: 2025/12/01 16:38:13 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	test_fill_tokens(int argc, char **argv)
{
	t_token	*tokens;
	size_t	token_count;

	(void)argc;
	printf("Fill Tokens Test:\n");
	printf("Input: %s\n", argv[2]);
	token_count = count_tokens(argv[2]);
	tokens = malloc(sizeof(t_token) * (token_count + 1));
	if (!tokens)
		return (printf("Memory allocation failed\n"), 1);
	set_token(&tokens[token_count], \
				(t_token_data){TOKEN_EOF, NULL, false, false});
	if (fill_tokens(argv[2], tokens) != 0)
	{
		printf("fill_tokens failed\n");
		free(tokens);
		return (1);
	}
	printf("Output Tokens:\n");
	print_tokens(tokens);
	printf("Fill Tokens Test Completed Successfully\n");
	free_tokens(tokens);
	return (0);
}
