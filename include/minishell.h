/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:07:18 by alisseye          #+#    #+#             */
/*   Updated: 2025/12/11 13:18:02 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// readline, rl_clear_history, rl_on_new_line,
// rl_replace_line, rl_redisplay, add_history
# include <readline/history.h>
# include <readline/readline.h>

// printf, perror, strerror
# include <stdio.h>

// malloc, free, getenv
# include <stdlib.h>

// write, access, open, read, close,
// unlink, dup, dup2, execve, getcwd, chdir,
// isatty, ttyname, ttyslot
# include <unistd.h>

// pipe, ioctl, tcsetattr, tcgetattr
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
# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h> // Библиотеки, которые ты забыл включить
# include <stdbool.h>
# include <string.h>
# include <sys/stat.h>

// SIZE_MAX
# include <stddef.h>

# define SUCCESS 0
# define FAILURE 1

typedef enum e_token_type
{
	TOKEN_NONE = 0,
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
}					t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	bool			to_expand;
	bool			quoted;
}					t_token;

typedef enum e_redirect_type
{
	REDIRECT_IN,
	REDIRECT_OUT,
	REDIRECT_APPEND,
	REDIRECT_HEREDOC,
}					t_redirect_type;

typedef struct s_redirect
{
	t_redirect_type	type;
	char			*target;
	int				heredoc_fd;
	bool			heredoc_quoted;
}					t_redirect;

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_OR,
	NODE_AND,
	NODE_SUBSHELL,
}					t_node_type;

typedef struct s_node
{
	t_node_type		type;
	char			**argv;
	size_t			argc;
	t_redirect		*redirects;
	size_t			redirects_count;
	struct s_node	*left;
	struct s_node	*right;
	struct s_node	*child;
}					t_node;

typedef struct s_shell_state
{
	t_node		*token_tree;
	char		**envp;
	int			last_exit_code;
}					t_shell_state;

// builtins

// built_in_cd.c
int					update_pwd_oldpwd(t_shell_state *state, char *old_pwd);
char				*get_cd_path(t_node *node, t_shell_state *state);
int					built_in_cd(t_node *node, t_shell_state *state);
int					built_in_cd2(t_node *node, t_shell_state *state,
						char *old_pwd);

// built_in_echo.c
bool				is_option(const char *str);
int					built_in_echo(t_node *node);

// built_in_env.c
int					built_in_env(t_node *node, t_shell_state *state);

// built_in_exit.c
int					check_overflow(unsigned long long n, char c_digit,
						int sign);
int					is_valid_llong(char *str);
int					free_all_resources(t_shell_state *state);
long long			ft_atoll(const char *str);
int					built_in_exit(t_node *node, t_shell_state *state);

// built_in_export.c
int					add_env_var(t_shell_state *state, char *var);
int					set_env_var(t_shell_state *state, char *var);
int					handle_export_args(t_node *node, t_shell_state *state);
int					built_in_export(t_node *node, t_shell_state *state);

// built_in_export_utils.c
void				sort_envp_copy(char **env_copy);
int					sort_env(t_shell_state *state);
int					print_method(char **env_copy, char *equal_sign, int i);
int					is_valid_identifier(char *str);

// built_in_pwd.c
int					built_in_pwd(t_node *node);

// built_in_unset.c
void				remove_var(int index, t_shell_state *state);
int					built_in_unset(t_node *node, t_shell_state *state);

// env

// env.c
char				**init_env(char **envp);
void				print_env(char **envp);
void				free_env(char **envp);

// env_utils.c
char				*get_key_from_var(char *var);
int					find_env_var_index(char **envp, char *key);
int					set_new_env_var(t_shell_state *state, char *key,
						char *value);

// execute_bin

// execute_bin.c
char				*find_in_path(char *cmd, t_shell_state *state);
char				*check_one_path(t_shell_state *state, char **paths,
						char *path, char *cmd);
void				check_path_validity(t_shell_state *state, char *path);
void				execute_bin(t_node *node, t_shell_state *state);
void				exit_failed_bin(t_shell_state *state, char *cmd,
						char *path);

// execute_ben_utils.c
void				free_split_array(char **arr);
char				*find_env_var_value(char *key, t_shell_state *state);

// executor

// executor.c
bool				is_built_in(t_node *node);
void				execute_built_in(t_node *node, t_shell_state *state);
void				start_built_in(t_node *node, t_shell_state *state);
void				execute_external(t_node *node, t_shell_state *state);
void				execute_ast(t_node *node, t_shell_state *state);

// subshell_or_and.c
void				execute_subshell(t_node *node, t_shell_state *state);
void				execute_and(t_node *node, t_shell_state *state);
void				execute_or(t_node *node, t_shell_state *state);

// pipe

// pipe.c
void				create_left_child(t_shell_state *state, t_node *node,
						int fd[2]);
void				create_right_child(t_shell_state *state, t_node *node,
						int fd[2]);
void				execute_pipe(t_node *node, t_shell_state *state);

// pipe_utils.c
void				handle_fork_error(t_shell_state *state, int fd[2]);
void				wait_child_processes(t_shell_state *state, pid_t left,
						pid_t right, int fd[2]);
void				kill_first_process(t_shell_state *state, int fd[2],
						pid_t left);

// redirects

// redirects.c
int					create_redirect_fd(t_redirect *redir);
int					apply_redirects(t_node *node);
void				restore_stdio(int saved_stdin, int saved_stdout);

// utils

int					error_msg(char *cmd, char *arg, char *custom_msg,
						int exit_code);

// env utils
char	*get_env_value(char *key, char **envp);

// lexer
int		tokenize(char *line, t_token **tokens, t_shell_state *state);

// utils
bool	is_whitespace(char c);
bool	is_operator_char(char c);
char	*ft_strndup(const char *s1, size_t n);
char	*ft_strnjoin(char *s1, char *s2, size_t n);

#endif
