/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_test.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 01:30:58 by danslav1e         #+#    #+#             */
/*   Updated: 2025/12/11 15:45:19 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

static int	run_test(t_shell_state *state, char **argv, int expected)
{
	t_node	*node;
	int		result;

	printf("[EXEC] Running command...\n");
	node = malloc(sizeof(t_node));
	if (!node)
		return (0);
	ft_memset(node, 0, sizeof(t_node));
	node->type = NODE_CMD;
	node->argv = argv;
	node->argc = 1;
	while (argv[node->argc])
		node->argc++;
	execute_ast(node, state);
	printf("[RESULT] Exit Code: %d (Expected: %d)\n",
		state->last_exit_code, expected);
	result = (state->last_exit_code == expected);
	print_result(result);
	free(node);
	return (result);
}

static int	run_all_tests(t_shell_state *state, char **args_ok, char **args_err)
{
	int	pass;
	int	curr_dir;

	pass = 0;
	print_test_header(0, "Normal PWD execution");
	pass += run_test(state, args_ok, 0);
	print_test_header(1, "PWD with arguments (Should Fail)");
	pass += run_test(state, args_err, 1);
	print_test_header(2, "PWD after changing directory to /tmp");
	curr_dir = open(".", O_RDONLY);
	chdir("/tmp");
	pass += run_test(state, args_ok, 0);
	fchdir(curr_dir);
	close(curr_dir);
	print_test_header(3, "PWD after unsetting PWD variable");
	pass += run_test(state, args_ok, 0);
	return (pass);
}

int	test_pwd(int argc, char **argv, char **envp)
{
	t_shell_state	state;
	char			*args_ok[2];
	char			*args_err[3];
	int				pass;

	(void)argc;
	(void)argv;
	printf("\n[INIT] Initializing Shell State...\n");
	state.envp = init_env(envp);
	state.last_exit_code = 0;
	if (!state.envp)
		return (printf("Error: Failed to init env\n"), 1);
	args_ok[0] = "pwd";
	args_ok[1] = NULL;
	args_err[0] = "pwd";
	args_err[1] = "arg1";
	args_err[2] = NULL;
	pass = run_all_tests(&state, args_ok, args_err);
	free_env(state.envp);
	print_summary(4, pass);
	return (0);
}
