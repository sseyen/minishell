/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens_test.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: autogen <autogen@example.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 18:40:00 by autogen           #+#    #+#             */
/*   Updated: 2025/12/11 18:40:00 by autogen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/test.h"
#include "../../src/lexer/lexer.h"

static int	match_value(t_token *tokens, char *expected)
{
	if (!tokens || !expected)
		return (0);
	if (tokens[1].type == TOKEN_EOF || tokens[1].type == TOKEN_NONE)
		return (0);
	if (!tokens[1].value)
		return (0);
	if (ft_strlen(tokens[1].value) != ft_strlen(expected))
		return (0);
	return (ft_strncmp(tokens[1].value, expected, ft_strlen(expected)) == 0);
}

static void	cleanup_case(t_token *tokens, char *expected, char **env_copy)
{
	if (expected)
		free(expected);
	if (tokens)
		free_tokens(tokens);
	if (env_copy)
		free_env(env_copy);
}

static char	*build_three_homes(char **envp)
{
	char	*home;
	char	*tmp;
	char	*expected;

	home = get_env_value("HOME", envp);
	if (!home)
		return (NULL);
	tmp = ft_strjoin(home, home);
	if (!tmp)
	{
		free(home);
		return (NULL);
	}
	expected = ft_strjoin(tmp, home);
	free(tmp);
	free(home);
	return (expected);
}

static char	*build_literal_and_home(char **envp)
{
	char	*home;
	char	*expected;

	home = get_env_value("HOME", envp);
	if (!home)
		return (NULL);
	expected = ft_strjoin("$HOME", home);
	free(home);
	return (expected);
}

static char	*build_empty(void)
{
	return (ft_strdup(""));
}

static int	run_expand(char *line, char *expected, char **envp)
{
	t_shell_state	state;
	t_token			*tokens;
	int				success;

	tokens = NULL;
	state.envp = init_env(envp);
	state.token_tree = NULL;
	state.last_exit_code = 0;
	if (!state.envp || !expected)
	{
		cleanup_case(tokens, expected, state.envp);
		print_result(0);
		return (0);
	}
	if (tokenize(line, &tokens, &state) != 0 || !tokens)
	{
		cleanup_case(tokens, expected, state.envp);
		print_result(0);
		return (0);
	}
	success = match_value(tokens, expected);
	print_result(success);
	cleanup_case(tokens, expected, state.envp);
	return (success);
}

static int	run_expand_case(char *line, char *env_key, char **envp)
{
	char	*expected;

	expected = get_env_value(env_key, envp);
	return (run_expand(line, expected, envp));
}

int	test_expand_tokens(int argc, char **argv, char **envp)
{
	int		pass;
	char	*expected;

	(void)argc;
	(void)argv;
	pass = 0;
	printf("\n[INIT] Testing expand_tokens...\n");
	print_test_header(0, "Simple env expansion");
	pass += run_expand_case("echo $PWD", "PWD", envp);
	print_test_header(1, "User expansion");
	pass += run_expand_case("echo $USER", "USER", envp);
	print_test_header(2, "Concatenate HOME with quotes");
	expected = build_three_homes(envp);
	pass += run_expand("echo $HOME\"$HOME\"$HOME", expected, envp);
	print_test_header(3, "Single-quoted literal + HOME");
	expected = build_literal_and_home(envp);
	pass += run_expand("echo '$HOME'$HOME", expected, envp);
	print_test_header(4, "Missing var expands to empty");
	expected = build_empty();
	pass += run_expand("echo $DOES_NOT_EXIST", expected, envp);
	print_summary(5, pass);
	return (0);
}
