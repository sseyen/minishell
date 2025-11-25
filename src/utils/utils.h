/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 19:42:59 by alisseye          #+#    #+#             */
/*   Updated: 2025/11/23 20:04:09 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <minishell.h>

char	*ft_strljoin(char *s1, char *s2);
char	*ft_strndup(const char *s, size_t n);
bool	is_whitespace(char c);
bool	is_operator_char(char c);

#endif