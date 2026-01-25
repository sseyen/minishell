/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 00:41:16 by danslav1e         #+#    #+#             */
/*   Updated: 2026/01/25 22:21:11 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** HANDLER: Интерактивный режим (ждем ввода в промпте)
** SIGINT (Ctrl+C):
** 1. g_signal = 130 (код выхода)
** 2. Пишем \n
** 3. Чистим текущую строку readline
** 4. Перерисовываем промпт
*/
static void	handler_interactive(int sig)
{
	(void)sig;
	g_signal = 130;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/*
** Установка сигналов для MAIN LOOP
*/
void	setup_signals_interactive()
{
	struct sigaction	sa;

	disable_echo_ctl();
	ft_bzero(&sa, sizeof(sa));
	
	// Обработка SIGINT
	sa.sa_handler = handler_interactive;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0; // Можно SA_RESTART, но readline сам справляется
	sigaction(SIGINT, &sa, NULL);

	// Игнорирование SIGQUIT (Ctrl+\)
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGQUIT, &sa, NULL);
}

/*
** Установка сигналов перед FORK (для Executor)
** Родитель будет игнорировать сигналы, пока ждет ребенка.
** А ребенок сбросит их в дефолт (SIG_DFL) внутри execute_bin.
*/
void	setup_signals_exec()
{
	struct sigaction	sa;

	ft_bzero(&sa, sizeof(sa));
	
	// Игнорируем оба сигнала в родителе во время waitpid
	// (чтобы шелл не падал, когда убиваем cat)
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

/*
** HANDLER: Heredoc
** Нам нужно прервать readline внутри heredoc.
** Самый надежный способ в 42 — закрыть stdin (fd 0).
** Это заставит readline вернуть NULL.
*/
static void	handler_heredoc(int sig)
{
	(void)sig;
	g_signal = 130; // Код прерывания
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO); // Ключевой момент: закрываем ввод
}

void	setup_signals_heredoc()
{
	struct sigaction	sa;

	disable_echo_ctl();
	ft_bzero(&sa, sizeof(sa));

	sa.sa_handler = handler_heredoc;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);

	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

void	setup_signals_child(void)
{
	struct sigaction	sa;

	ft_bzero(&sa, sizeof(sa));
	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}