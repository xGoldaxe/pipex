/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleveque <pleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/15 16:24:13 by pleveque          #+#    #+#             */
/*   Updated: 2022/01/18 14:12:54 by pleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	write_command_output(int pipe_fd, char *output)
{
	char	buffer[1024];
	int		ret;
	int		fd;

	unlink(output);
	fd = open(output, O_WRONLY | O_CREAT, S_IRWXU);
	if (fd == -1)
	{
		perror("open");
		return (-1);
	}
	printf("\n<---OUTPUT--->\n");
	ret = 1;
	while (ret > 0)
	{
		ret = read(pipe_fd, buffer, 1024);
		write(fd, buffer, ret);
		write(1, buffer, ret);
	}
	printf("\n<---//////--->\n");
	close(fd);
	close(pipe_fd);
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

	parsed_cmd = ft_split(command, ' ');
	if (!parsed_cmd)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(full_path, parsed_cmd[0]);
		if (access(full_path, F_OK | X_OK) == 0)
		{
			free(parsed_cmd[0]);
			parsed_cmd[0] = full_path;
			return (parsed_cmd);
		}
	}
	perror("invalid command");
	return (NULL);
}

int	iter_pipes(int argc, char **argv, char **env, char **paths)
{
	pid_t	pid;
	int		first_pipe;
	int		new_pipe_fd[2];
	int		arg_i;
	char	**p_cmd;

	first_pipe = first_cmd(argv, paths);
	if (first_pipe == -1)
		return (-1);
	arg_i = 2;
	while (argv[++arg_i + 1])
	{
		p_cmd = parse_cmd(argv[arg_i], paths);
		if (!p_cmd || pipe(new_pipe_fd) == -1)
			return (-1);
		pid = fork();
		if (pid == -1)
			return (-1);
		else if (pid == 0)
			return (run_command(first_pipe, new_pipe_fd, p_cmd, env));
		close(new_pipe_fd[1]);
		waitpid(pid, 0, 0);
		free_split(p_cmd);
		if (dup2(new_pipe_fd[0], first_pipe) == -1)
			return (-1);
		if (argv[arg_i + 2])
			close(new_pipe_fd[0]);
	}
	return (write_command_output(new_pipe_fd[0], argv[argc - 1]));
}

char	**get_paths(char **env)
{
	int		i;
	char	**paths;

	i = -1;
	while (env[++i])
	{
		if (ft_strncmp(env[i], "PATH", 4) == 0)
		{
			paths = ft_split(env[i] + 5, ':');
			if (!paths)
				return (NULL);
			return (paths);
		}
	}
	return (NULL);
}

int	main(int argc, char **argv, char **env)
{
	char	**paths;

	paths = get_paths(env);
	if (!paths)
	{
		printf("env error\n");
		return (-1);
	}
	if (argc < 5)
	{
		printf("not enough args\n");
		return (-1);
	}
	if (access(argv[1], R_OK) == -1)
	{
		perror("input");
		return (-1);
	}
	if (access(argv[argc - 1], F_OK) == 0 && access(argv[argc - 1], W_OK) == -1)
	{
		perror("output");
		return (-1);
	}
	if (iter_pipes(argc, argv, env, paths) == -1)
	{
		perror("MAIN");
		return (-1);
	}
	return (1);
}
