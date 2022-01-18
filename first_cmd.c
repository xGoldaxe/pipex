/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleveque <pleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 16:59:01 by pleveque          #+#    #+#             */
/*   Updated: 2022/01/18 13:59:05 by pleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* use a command and send join it to new_pipe_fd[0] */

int	first_cmd(char **argv, char **paths)
{
	pid_t	pid;
	int		pipe_fd[2];
	int		new_entry;
	char	**parsed_cmd;

	parsed_cmd = parse_cmd(argv[2], paths);
	if (pipe(pipe_fd) == -1 || !parsed_cmd)
		return (-1);
	new_entry = open(argv[1], O_RDONLY);
	pid = fork();
	if (pid == -1 || new_entry == -1)
		return (-1);
	if (pid == 0)
		return (run_command(new_entry, pipe_fd, parsed_cmd, NULL));
	close(pipe_fd[1]);
	waitpid(pid, 0, 0);
	free_split(parsed_cmd);
	return (pipe_fd[0]);
}
