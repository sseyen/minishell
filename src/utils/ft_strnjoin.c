/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnjoin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 17:18:30 by alisseye          #+#    #+#             */
/*   Updated: 2025/12/14 16:58:18 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
