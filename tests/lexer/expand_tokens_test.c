/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens_test.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 16:22:05 by alisseye          #+#    #+#             */
/*   Updated: 2025/12/01 16:53:54 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	test_expand_tokens(int argc, char **argv, char **envp)
{
	t_token			*tokens;
	t_shell_state	state;

	(void)argc;
	state.envp = envp;
	if (tokenize(argv[2], &tokens, &state) != 0)
		return (1);
	print_tokens(tokens);
	return (0);
}
