/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 05:19:38 by alisseye          #+#    #+#             */
/*   Updated: 2025/12/01 16:52:20 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
# define TEST_H

# include "minishell.h"
# include "lexer.h"

int	test_init_env(int argc, char **argv, char **envp);
int	test_count_tokens(int argc, char **argv);
int	test_fill_tokens(int argc, char **argv);
int	test_expand_tokens(int argc, char **argv, char **envp);

// Utils
int	print_tokens(t_token *tokens);

#endif
