/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnjoin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 17:18:30 by alisseye          #+#    #+#             */
/*   Updated: 2026/01/26 01:52:07 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief
 * Helper to copy characters from src to dst at given position.
 */
static void	copy_part(char *dst, char *src, size_t *pos, size_t limit)
{
	size_t	j;

	j = 0;
	while (src && src[j] && j < limit)
	{
		dst[*pos + j] = src[j];
		j++;
	}
	*pos += j;
}

/**
 * @brief
 * Joins s1 with first n characters of s2. Frees s1.
 *
 * @param s1 First string (freed after join).
 * @param s2 Second string.
 * @param n Number of characters from s2 to append.
 * @return Newly allocated joined string.
 */
char	*ft_strnjoin(char *s1, char *s2, size_t n)
{
	char	*new_str;
	size_t	pos;
	size_t	len1;

	if (!s1 && !s2)
		return (NULL);
	len1 = 0;
	if (s1)
		len1 = ft_strlen(s1);
	new_str = malloc(sizeof(char) * (len1 + n + 1));
	if (!new_str)
		return (NULL);
	pos = 0;
	copy_part(new_str, s1, &pos, len1);
	copy_part(new_str, s2, &pos, n);
	new_str[pos] = '\0';
	if (s1)
		free(s1);
	return (new_str);
}
