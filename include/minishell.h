/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:07:18 by alisseye          #+#    #+#             */
/*   Updated: 2025/10/02 15:41:34 by alisseye         ###   ########.fr       */
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

# include <stdbool.h>

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_REDIRECT_HEREDOC,
	TOKEN_OR,
	TOKEN_AND,
	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_EOF,
	TOKEN_ERROR,
}	t_token_type;

typedef struct s_token
{
	t_token_type		type;
	char				*value;
	bool				to_expand;
}	t_token;

typedef enum e_redirect_type
{
	REDIRECT_IN,
	REDIRECT_OUT,
	REDIRECT_APPEND,
	REDIRECT_HEREDOC,
}	t_redirect_type;

typedef struct s_redirect
{
	t_redirect_type		type;
	char				*target;
	bool				to_expand;
}	t_redirect;

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_OR,
	NODE_AND,
	NODE_SUBSHELL,
}	t_node_type;

typedef struct s_node
{
	t_node_type		type;
	char			**argv;
	t_redirect		*redirects;
	struct s_node	*left;
	struct s_node	*right;
	struct s_node	*child;
}	t_node;

#endif
