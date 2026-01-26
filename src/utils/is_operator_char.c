/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_operator_char.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 21:50:02 by alisseye          #+#    #+#             */
/*   Updated: 2026/01/26 01:52:07 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief
 * Checks if character is a shell operator: | < > ( ) &
 */
bool	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '(' || c == ')'
		|| c == '&');
}
