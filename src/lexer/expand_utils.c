/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 15:36:47 by alisseye          #+#    #+#             */
/*   Updated: 2025/12/01 15:39:58 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

char	*join_strings(char *s1, char *s2, size_t from, size_t to)
{
	char	*temp;

	if (to - from == 0)
		return (s1);
	temp = ft_strnjoin(s1, &s2[from], to - from);
	if (!temp)
	{
		free(s1);
		return (NULL);
	}
	free(s1);
	return (temp);
}

char	*parse_var(char *str, t_shell_state *state, size_t *index)
{
	size_t	i;
	char	*var_name;
	char	*var_value;

	i = 0;
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
	var_value = get_env_value(state->envp, var_name);
	free(var_name);
	*index += i;
	if (!var_value)
		return (ft_strdup(""));
	return (var_value);
}

char	*handle_var(char *token_value, size_t *from, size_t *i, \
	t_shell_state *state)
{
	char	*var_value;
	char	*new_value;

	(*i)++;
	new_value = join_strings(new_value, token_value, *from, *i);
	if (!new_value)
		return (1);
	var_value = parse_var(&token_value[*i + 1], state, i);
	if (!var_value)
		return (1);
	*from = *i;
	new_value = join_strings(new_value, var_value, \
		0, ft_strlen(var_value));
	free(var_value);
	if (!new_value)
		return (NULL);
	return (new_value);
}

int	handle_eof(char *token_value, char **new_value, \
	size_t from, size_t i)
{
	char	*temp;

	temp = NULL;
	temp = join_strings(*new_value, token_value, from, i);
	if (!temp)
		return (1);
	free(*new_value);
	*new_value = temp;
	return (0);
}
