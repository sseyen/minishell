/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 18:55:25 by danslav1e         #+#    #+#             */
/*   Updated: 2026/01/15 20:05:41 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** Обработчик SIGINT (Ctrl+C) для интерактивного режима.
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
** Настройка сигналов для режима ожидания ввода (строка приглашения).
** Ctrl+C -> handler_interactive
** Ctrl+\ -> IGNORE
*/
void	setup_signals_interactive(void)
{
	struct sigaction	sa;

	disable_echo_ctl();
	ft_bzero(&sa, sizeof(sa));
	
	// SIGINT
	sa.sa_handler = handler_interactive;
	sigemptyset(&sa.sa_mask);
	// SA_RESTART нужен, чтобы системные вызовы (read) перезапускались,
	// но readline это обрабатывает сам. Можно ставить 0.
	sa.sa_flags = 0; 
	sigaction(SIGINT, &sa, NULL);

	// SIGQUIT
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGQUIT, &sa, NULL);
}

/*
** Настройка сигналов для дочернего процесса.
** Возвращаем стандартное поведение (Default).
*/
void	setup_signals_child(void)
{
	struct sigaction	sa;

	ft_bzero(&sa, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_DFL;

	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

/*
** Настройка сигналов для родителя, пока он ждет завершения детей.
** Мы игнорируем сигналы, чтобы не дублировать действия детей.
*/
void	setup_signals_parent_wait(void)
{
	struct sigaction	sa;

	ft_bzero(&sa, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_IGN;

	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

/*
** Обработчик для Heredoc.
** Нам нужно прервать readline. Самый простой способ в minishell — закрыть stdin.
** Но это требует аккуратного восстановления stdin потом.
** Либо использовать rl_event_hook (сложно).
**
** Простой вариант: ставим g_signal и пишем \n. Readline сам вернет управление
** после нажатия Enter, либо используем ioctl для инъекции \n.
**
** Но для school 42 часто используют трюк с закрытием (0).
*/
static void	handler_heredoc(int sig)
{
	(void)sig;
	g_signal = 130;
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO); // Прерываем readline
}

void	setup_signals_heredoc(void)
{
	struct sigaction	sa;

	ft_bzero(&sa, sizeof(sa));
	sa.sa_handler = handler_heredoc;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	
	sigaction(SIGINT, &sa, NULL);
	// SIGQUIT в heredoc игнорируется
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}
