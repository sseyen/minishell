/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danslav1e <danslav1e@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 05:19:38 by alisseye          #+#    #+#             */
/*   Updated: 2026/01/14 19:18:35 by danslav1e        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
# define TEST_H

# include <fcntl.h>
# include <unistd.h>

# include "minishell.h"

int		test_init_env(int argc, char **argv, char **envp);
int		test_count_tokens(int argc, char **argv);
int		test_fill_tokens(int argc, char **argv);
int		test_expand_tokens(int argc, char **argv, char **envp);
int		test_pwd(int argc, char **argv, char **envp);
int		test_build_ast(int argc, char **argv, char **envp);
void	print_ast(t_node *root);

// Utils
int		print_tokens(t_token *tokens);
void	print_test_header(int index, char *desc);
void	print_result(int success);
void	print_summary(int total, int passed);

#endif
