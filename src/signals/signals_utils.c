/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 00:00:00 by danslav1e         #+#    #+#             */
/*   Updated: 2026/01/26 01:52:06 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief
 * Signal handler for SIGINT during heredoc input.
 * Closes stdin to interrupt readline and sets g_signal to 130.
 */
static void	handler_heredoc(int sig)
{
	(void)sig;
	g_signal = 130;
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
}

/**
 * @brief
 * Sets up signal handlers for heredoc input.
 * SIGINT: closes stdin to exit readline.
 * SIGQUIT: ignored.
 */
void	setup_signals_heredoc(void)
{
	struct sigaction	sa;

	ft_bzero(&sa, sizeof(sa));
	sa.sa_handler = handler_heredoc;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

/**
 * @brief
 * Restores default signal handlers in child process.
 * Both SIGINT and SIGQUIT are reset to SIG_DFL.
 */
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
