/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_tokens_test.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 04:42:47 by alisseye          #+#    #+#             */
/*   Updated: 2025/10/27 12:36:32 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	test_count_tokens(int argc, char **argv)
{
	(void)argc;
	printf("Count Tokens Test:\n");
	printf("Input: %s\n", argv[2]);
	printf("Output: %zu\n", count_tokens(argv[2]));
	return (0);
}
