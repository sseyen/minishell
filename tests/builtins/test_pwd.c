/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 01:30:58 by danslav1e         #+#    #+#             */
/*   Updated: 2025/12/03 12:38:31 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Структура для описания одного тест-кейса
typedef struct s_test_case {
	char	*description;
	char	**argv;        // Аргументы команды (pwd, arg1...)
	int		expected_code; // Ожидаемый код возврата
	char	*setup_action; // "cd", "unset" или NULL
}	t_test_case;

/*
** Вспомогательная функция для создания узла AST вручную
*/
t_node	*create_cmd_node(char **argv)
{
	t_node	*node;
	int		i;

	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	ft_memset(node, 0, sizeof(t_node));
	node->type = NODE_CMD;
	node->argv = argv;
	// Считаем argc
	i = 0;
	while (argv[i])
		i++;
	node->argc = i;
	return (node);
}

/*
** Красивый вывод заголовка теста
*/
void	print_test_header(int index, char *desc)
{
	printf("\n**************************************************\n");
	printf("* TEST %d: %s\n", index + 1, desc);
	printf("**************************************************\n");
}

int	main(int argc, char **argv, char **envp)
{
	t_shell_state	state;
	t_node			*node;
	int				i;

	(void)argc;
	(void)argv;

	// 1. Инициализация
	printf("\n[INIT] Initializing Shell State...\n");
	state.envp = init_env(envp);
	state.last_exit_code = 0;
	if (!state.envp)
		return (printf("Error: Failed to init env\n"), 1);

	// 2. Подготовка данных для тестов
	// Аргументы для разных ситуаций
	char *args_normal[] = {"pwd", NULL};
	char *args_error[] = {"pwd", "arg1", NULL}; // Лишний аргумент
	
	// Массив сценариев
	t_test_case tests[] = {
		{
			"Normal PWD execution",
			args_normal,
			0,
			NULL
		},
		{
			"PWD with arguments (Should Fail)",
			args_error,
			1,
			NULL
		},
		{
			"PWD after changing directory to /tmp",
			args_normal,
			0,
			"cd /tmp"
		},
		{
			"PWD after unsetting PWD variable",
			args_normal,
			0,
			"unset PWD"
		}
	};
	int num_tests = sizeof(tests) / sizeof(tests[0]);

	// 3. Цикл выполнения тестов
	i = 0;
	while (i < num_tests)
	{
		print_test_header(i, tests[i].description);

		// Предварительная настройка (Setup)
		if (tests[i].setup_action)
		{
			printf("[SETUP] Performing: %s\n", tests[i].setup_action);
			if (ft_strncmp(tests[i].setup_action, "cd", 2) == 0)
			{
				// Эмулируем работу built_in_cd системным вызовом для теста
				chdir("/tmp");
				// В реальной жизни тут надо обновить env через update_env_vars,
				// но для теста pwd достаточно chdir, так как pwd берет getcwd.
			}
			else if (ft_strncmp(tests[i].setup_action, "unset", 5) == 0)
			{
				// Эмулируем unset
				int idx = find_env_var_index(state.envp, "PWD");
				if (idx != -1) remove_var(idx, &state);
			}
		}

		// Создаем и запускаем
		printf("[EXEC] Running command...\n");
		node = create_cmd_node(tests[i].argv);
		
		execute_ast(node, &state);
		
		printf("[RESULT] Exit Code: %d (Expected: %d)\n", 
			state.last_exit_code, tests[i].expected_code);

		if (state.last_exit_code == tests[i].expected_code)
			printf("✅ SUCCESS\n");
		else
			printf("❌ FAILURE\n");
		printf("%ld\n", node->argc);
		free(node); // Очистка узла (но не строк argv, они на стеке)
		i++;
	}

	// 4. Финальная очистка
	printf("\n[CLEANUP] Freeing environment...\n");
	free_env(state.envp);
	
	return (0);
}
