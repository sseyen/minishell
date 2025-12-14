/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_tokens_test.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 18:30:00 by autogen           #+#    #+#             */
/*   Updated: 2025/12/14 17:19:59 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/test.h"
#include "../../src/lexer/lexer.h"

static int	run_case(char *line, size_t expected)
{
	size_t	count;

	count = count_tokens(line);
	if (count == expected)
	{
		print_result(1);
		return (1);
	}
	print_result(0);
	return (0);
}

int	test_count_tokens(int argc, char **argv)
{
	int		passed;

	(void)argc;
	(void)argv;
	passed = 0;
	printf("\n[INIT] Testing count_tokens...\n");
	print_test_header(0, "Simple words and spaces");
	passed += run_case("echo hello world", 3);
	print_test_header(1, "With pipe and redir");
	passed += run_case("ls -l | grep txt > out", 7);
	print_test_header(2, "Quoted string counts as one token");
	passed += run_case("echo \"hello world\"", 2);
	print_test_header(3, "Unmatched quote returns SIZE_MAX");
	passed += run_case("echo \"hello world", SIZE_MAX);
	print_test_header(4, "Mixed quotes and words");
	passed += run_case("echo hello'hello'world", 2);
	print_test_header(5, "Filename with spaces in quotes");
	passed += run_case("cat \"file name with spaces.txt\"", 2);
	print_test_header(6, "Multiple operators and spaces");
	passed += run_case("ls   -a  >>   output.txt  |  wc -l", 7);
	print_test_header(7, "Empty input");
	passed += run_case("", 0);
	print_test_header(8, "Only spaces");
	passed += run_case("     ", 0);
	print_test_header(9, "No spaces between tokens");
	passed += run_case("echo|wc -l", 4);
	print_test_header(10, "Consecutive operators");
	passed += run_case("echo||<<>>file", 5);
	print_test_header(11, "Many consecutive operators");
	passed += run_case("echo|||||<<>>file", 7);
	print_summary(12, passed);
	return (0);
}
