/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnjoin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 17:18:30 by alisseye          #+#    #+#             */
/*   Updated: 2025/11/29 17:19:55 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

char	*ft_strnjoin(const char *s1, const char *s2, size_t n)
{
	size_t	i;
	size_t	j;
	char	*new_str;

	if (!s1 && !s2)
		return (NULL);
	new_str = malloc(sizeof(char) * (ft_strlen(s1) + n + 1));
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	while (s1 && s1[i])
	{
		new_str[i] = s1[i];
		i++;
	}
	while (s2 && j < n && s2[j])
	{
		new_str[i + j] = s2[j];
		j++;
	}
	new_str[i + j] = '\0';
	free(s1);
	return (new_str);
}
