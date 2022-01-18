/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleveque <pleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 18:47:03 by pleveque          #+#    #+#             */
/*   Updated: 2022/01/18 19:26:53 by pleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

void	*create_leaks(void)
{
	void	*test;

	test = malloc(10000);
	return test;
}

int	main(void)
{
	void	*test;

	printf("test\n");
	test = create_leaks();
	test = create_leaks();
	create_leaks();
	create_leaks();
	create_leaks();
	create_leaks();
	create_leaks();
	create_leaks();
	create_leaks();
	create_leaks();
	create_leaks();
	create_leaks();
	create_leaks();
	while(1)
	{

	}
	return (-1);
}