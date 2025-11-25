/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 16:04:23 by alisseye          #+#    #+#             */
/*   Updated: 2025/11/25 16:05:20 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	expand_tokens(t_token *tokens, t_shell_state *state)
{
	size_t	i;

	while (tokens[i].type != TOKEN_EOF || tokens[i].type != TOKEN_NONE)
	{
		if (tokens[i].to_expand && !tokens[i].quoted)
		{
			// Perform expansion using state->envp and state->last_exit_code
			// Update tokens[i].value accordingly
		}
		i++;
	}
	return (0);
}
