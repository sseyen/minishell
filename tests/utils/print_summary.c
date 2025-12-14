/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_summary.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 15:43:38 by alisseye          #+#    #+#             */
/*   Updated: 2025/12/11 15:43:47 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void	print_summary(int total, int passed)
{
	printf("\n[CLEANUP]\n");
	printf("\n***** TEST SUMMARY *****\n");
	printf("Total: %d, Passed: %d, Failed: %d\n", total, passed,
		total - passed);
}
