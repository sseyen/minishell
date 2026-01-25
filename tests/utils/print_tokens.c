/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 16:38:26 by alisseye          #+#    #+#             */
/*   Updated: 2026/01/25 00:13:45 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <test.h>

int	print_tokens(t_token *tokens)
{
	int	i;

	i = 0;
	while (tokens[i].type != TOKEN_NONE && tokens[i].type != TOKEN_EOF)
	{
		printf("Token %d: Type=%d, Value='%s'\n", i, tokens[i].type,
			tokens[i].value);
		i++;
	}
	if (tokens[i].type == TOKEN_EOF)
		printf("Token %d: Type=%d, Value='EOF'\n", i, tokens[i].type);
	else if (tokens[i].type == TOKEN_NONE)
		printf("Token %d: Type=%d, Value='NONE'\n", i, tokens[i].type);
	return (0);
}
