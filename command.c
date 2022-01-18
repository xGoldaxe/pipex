/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleveque <pleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 19:13:26 by pleveque          #+#    #+#             */
/*   Updated: 2022/01/18 19:29:18 by pleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	write_command_output(int pipe_fd, char *output, int add_to)
{
	char	buffer[1023];
	int		ret;
	int		fd;

	if (add_to == 0)
		unlink(output);
	fd = open(output, O_RDWR | O_CREAT, S_IRWXU);
	if (fd == -1)
	{
		perror("open");
		return (-1);
	}
	ret = 1023;
	while (ret == 1023)
		ret = read(fd, buffer, 1023);
	ret = 1;
	while (ret > 0)
	{
		ret = read(pipe_fd, buffer, 1023);
		write(fd, buffer, ret);
	}
	close(fd);
	return (0);
}

int	run_command(int entry_pipe, int	*pipe_fd, char **parsed_cmd, char **env)
{
	if (dup2(entry_pipe, 0) == -1 || dup2(pipe_fd[1], 1) == -1)
		return (-1);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	close(entry_pipe);
	if (!parsed_cmd)
		perror("split error");
	if (execve(parsed_cmd[0], &parsed_cmd[0], env) == -1)
		perror("execve");
	return (-1);
}

char	**parse_cmd(char *command, char **paths)
{
	char	**parsed_cmd;
	int		i;
	char	*full_path;
	char	*tmp;

	parsed_cmd = ft_split(command, ' ');
	if (!parsed_cmd)
		return (NULL);
	if (access(parsed_cmd[0], F_OK | X_OK) == 0)
		return (parsed_cmd);
	i = -1;
	while (paths[++i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(tmp, parsed_cmd[0]);
		free(tmp);
		if (access(full_path, F_OK | X_OK) == 0)
		{
			free(parsed_cmd[0]);
			parsed_cmd[0] = full_path;
			return (parsed_cmd);
		}
		free(full_path);
	}
	input_error("Command", parsed_cmd[0], 2);
	free(parsed_cmd);
	return (NULL);
}
