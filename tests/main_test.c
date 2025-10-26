/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alisseye <alisseye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 05:15:32 by alisseye          #+#    #+#             */
/*   Updated: 2025/10/26 05:57:23 by alisseye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	main(int argc, char **argv, char **envp)
{
	if (ft_strncmp(argv[1], "env", 4) == 0)
		return (test_init_env(argc, argv, envp));
	else if (ft_strncmp(argv[1], "count_tokens", 13) == 0)
		return (test_count_tokens(argc, argv));
	return (0);
}
