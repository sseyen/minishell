/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 15:36:47 by alisseye          #+#    #+#             */
/*   Updated: 2026/01/26 21:30:30 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

/**
 * @brief
 * Joins s1 with substring of s2 (from..to). Frees s1.
 */
char	*join_strings(char *s1, char *s2, size_t from, size_t to)
{
	char	*temp;

	temp = ft_strnjoin(s1, &s2[from], to - from);
	if (!temp)
		return (NULL);
	return (temp);
}

/**
 * @brief
 * Parses variable name after $ and returns its value.
 * Handles $? for exit code.
 *
 * @param str String starting at variable name.
 * @param state Shell state.
 * @param index Updated with characters consumed.
 * @return Allocated value string.
 */
char	*parse_var(char *str, t_shell_state *state, size_t *index)
{
	size_t	i;
	char	*var_name;
	char	*var_value;

	i = 0;
	if (str[i] == '\0' || is_whitespace(str[i]))
		return (ft_strdup("$"));
	if (str[i] == '?')
	{
		(*index)++;
		return (ft_itoa(state->last_exit_code));
	}
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	var_name = ft_strndup(str, i);
	if (!var_name)
		return (NULL);
	var_value = get_env_value(var_name, state->envp);
	free(var_name);
	*index += i;
	if (!var_value)
		return (ft_strdup(""));
	return (var_value);
}

/**
 * @brief
 * Appends remaining text after last variable expansion.
 */
int	handle_eof(char *token_value, char **new_value, size_t from, size_t i)
{
	char	*temp;

	temp = NULL;
	temp = join_strings(*new_value, token_value, from, i);
	if (!temp)
		return (1);
	*new_value = temp;
	return (0);
}
