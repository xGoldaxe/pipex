/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleveque <pleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 16:59:01 by pleveque          #+#    #+#             */
/*   Updated: 2022/01/18 18:07:02 by pleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* use a command and send join it to new_pipe_fd[0] */

int	first_cmd(char **argv, char **env, char **paths)
{
	pid_t	pid;
	int		pipe_fd[2];
	int		new_entry;
	char	**parsed_cmd;

	if (pipe(pipe_fd) == -1)
		return (-1);
	parsed_cmd = parse_cmd(argv[2], paths);
	if (!parsed_cmd)
	{
		close(pipe_fd[1]);
		return (pipe_fd[0]);
	}
	new_entry = open(argv[1], O_RDONLY);
	if (new_entry == -1)
		return (-1);
	pid = fork();
	if (pid == -1 || new_entry == -1)
		return (-1);
	if (pid == 0)
		return (run_command(new_entry, pipe_fd, parsed_cmd, env));
	close(pipe_fd[1]);
	free_split(parsed_cmd);
	return (pipe_fd[0]);
}

int	limited_stdin(char **argv)
{
	char	*line;
	int		pipe_fd[2];
	int		first_iteration;

	if (pipe(pipe_fd) == -1)
		return (-1);
	first_iteration = 1;
	line = NULL;
	while (line || first_iteration)
	{
		line = get_next_line(0);
		if (!line || ft_strncmp(line, argv[3],
				ft_biggest(ft_strlen(argv[3]), ft_strclen(line, '\n'))) == 0)
		{
			close(pipe_fd[1]);
			close(pipe_fd[1]);
			return (pipe_fd[0]);
		}
		write(pipe_fd[1], line, ft_strlen(line));
		free(line);
		first_iteration = 0;
	}
	return (-1);
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}
