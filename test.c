/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleveque <pleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 15:48:21 by pleveque          #+#    #+#             */
/*   Updated: 2022/01/18 14:07:31 by pleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(void)
{
	void	*ptr;
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		printf("child adress is: %p\n", ptr);
	else
		printf("adress is: %p\n", ptr);
	return (0);
}
