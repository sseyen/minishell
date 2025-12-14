/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast_test.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: autogen <autogen@example.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 19:05:00 by autogen           #+#    #+#             */
/*   Updated: 2025/12/14 19:05:00 by autogen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/test.h"

static int	match_cmd(t_node *node, char *a0, char *a1)
{
	if (!node || node->type != NODE_CMD)
		return (0);
	if (!node->argv || !node->argv[0])
		return (0);
	if (ft_strncmp(node->argv[0], a0, ft_strlen(a0) + 1) != 0)
		return (0);
	if (a1 == NULL)
		return (1);
	if (!node->argv[1])
		return (0);
	return (ft_strncmp(node->argv[1], a1, ft_strlen(a1) + 1) == 0);
}

static int	check_simple(t_node *tree)
{
	return (match_cmd(tree, "echo", "hello") && tree->argc == 2);
}

static int	check_pipe(t_node *tree)
{
	if (!tree || tree->type != NODE_PIPE)
		return (0);
	if (!match_cmd(tree->left, "echo", "hi"))
		return (0);
	return (match_cmd(tree->right, "cat", NULL));
}

static int	check_or(t_node *tree)
{
	if (!tree || tree->type != NODE_OR)
		return (0);
	if (!match_cmd(tree->left, "false", NULL))
		return (0);
	return (match_cmd(tree->right, "echo", "ok"));
}

static int	check_subshell(t_node *tree)
{
	if (!tree || tree->type != NODE_AND)
		return (0);
	if (!tree->left || tree->left->type != NODE_SUBSHELL)
		return (0);
	if (!match_cmd(tree->left->child, "echo", "hi"))
		return (0);
	return (match_cmd(tree->right, "ls", NULL));
}

static int	run_case(char *line, int (*check)(t_node *), char **envp)
{
	t_shell_state	state;
	t_token			*tokens;
	int				ok;

	state.envp = init_env(envp);
	state.token_tree = NULL;
	state.last_exit_code = 0;
	if (!state.envp)
		return (print_result(0), 0);
	if (tokenize(line, &tokens, &state) != 0)
		return (free_env(state.envp), print_result(0), 0);
	if (build_ast(tokens, &state.token_tree) != 0 || !state.token_tree)
		return (free_env(state.envp), print_result(0), 0);
	ok = check(state.token_tree);
	print_result(ok);
	free_ast(state.token_tree);
	free_env(state.envp);
	return (ok);
}

int				test_build_ast(int argc, char **argv, char **envp)
{
	int	pass;

	(void)argc;
	(void)argv;
	pass = 0;
	printf("\n[INIT] Testing build_ast...\n");
	print_test_header(0, "Simple command");
	pass += run_case("echo hello", check_simple, envp);
	print_test_header(1, "Pipe command");
	pass += run_case("echo hi | cat", check_pipe, envp);
	print_test_header(2, "Or expression");
	pass += run_case("false || echo ok", check_or, envp);
	print_test_header(3, "Subshell and");
	pass += run_case("(echo hi) && ls", check_subshell, envp);
	print_summary(4, pass);
	return (0);
}
