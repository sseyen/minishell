/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:07:18 by alisseye          #+#    #+#             */
/*   Updated: 2025/10/01 15:35:30 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// readline, rl_clear_history, rl_on_new_line,
// rl_replace_line, rl_redisplay, add_history
# include <readline/readline.h>
# include <readline/history.h>

// printf, perror, strerror
# include <stdio.h>

// malloc, free, getenv
# include <stdlib.h>

// write, access, open, read, close,
// unlink, dup, dup2, execve, getcwd, chdir,
// isatty, ttyname, ttyslot
# include <unistd.h>

// pipe, ioctl, tcsetattr, tcgetattr
# include <unistd.h>
# include <sys/ioctl.h>
# include <termios.h>

// fork, wait, waitpid, wait3, wait4, exit
# include <sys/types.h>
# include <sys/wait.h>

// signal, sigaction, sigemptyset, sigaddset, kill
# include <signal.h>

// opendir, readdir, closedir
# include <dirent.h>

// stat, lstat, fstat
# include <sys/stat.h>



#endif
