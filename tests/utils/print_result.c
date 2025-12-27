/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_result.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 15:43:59 by alisseye          #+#    #+#             */
/*   Updated: 2025/12/11 15:44:06 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void	print_result(int success)
{
	if (success)
		printf("✅ SUCCESS\n");
	else
		printf("❌ FAILURE\n");
}
