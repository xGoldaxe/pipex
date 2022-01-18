/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleveque <pleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/15 16:24:13 by pleveque          #+#    #+#             */
/*   Updated: 2022/01/18 18:41:18 by pleveque         ###   ########.fr       */
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

	parsed_cmd = ft_split(command, ' ');
	if (!parsed_cmd)
		return (NULL);
	if (access(parsed_cmd[0], F_OK | X_OK) == 0)
		return (parsed_cmd);
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
	input_error("Command", parsed_cmd[0], 2);
	free(parsed_cmd[0]);
	return (NULL);
}

int	iter_pipes(int argc, char **argv, char **env, char **paths)
{
	pid_t	pid;
	int		first_pipe;
	int		new_pipe_fd[2];
	int		arg_i;
	char	**p_cmd;

	arg_i = 2;
	if (ft_strncmp(argv[2], "here_doc", 8) == 0)
	{
		first_pipe = limited_stdin(argv);
		++arg_i;
	}
	else
		first_pipe = first_cmd(argv, env, paths);
	if (first_pipe == -1)
		return (-1);
	while (argv[++arg_i + 1])
	{
		p_cmd = parse_cmd(argv[arg_i], paths);
		if (!p_cmd)
			write_command_output(first_pipe, argv[argc - 1], ft_strncmp(argv[2], "here_doc", 8) == 0);
		else
		{
			if (pipe(new_pipe_fd) == -1)
				return (-1);
			pid = fork();
			if (pid == -1)
				return (-1);
			else if (pid == 0)
				return (run_command(first_pipe, new_pipe_fd, p_cmd, env));
			close(new_pipe_fd[1]);
			free_split(p_cmd);
			if (dup2(new_pipe_fd[0], first_pipe) == -1)
				return (-1);
			if (argv[arg_i + 2])
				close(new_pipe_fd[0]);
		}
	}
	return (write_command_output(new_pipe_fd[0], argv[argc - 1], ft_strncmp(argv[2], "here_doc", 8) == 0));
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

int	input_error(char *error_type, char	*precision, int type)
{
	char	*error;

	error = ft_strjoin("\033[1;31m", error_type);
	if (!error)
		perror("Malloc failed");
	else
	{
		perror(error);
		printf("\033[0m");
		if (type == 1)
		{
			printf("Try: ./pipex <file1> <cmd1> <cmd2> <...> file2");
			printf("\n     ./pipex here_doc <LIMITER> <cmd> <cmd1> <...> file");
		}
		if (type == 2)
			printf("Invalid command: \033[1;35m%s\n\033[0m", precision);
		free(error);
	}
	return (1);
}

int	main(int argc, char **argv, char **env)
{
	char	**paths;
	int		fd;

	if (argc < 5)
		return (input_error("Arguments", NULL, 1));
	paths = get_paths(env);
	if (!paths)
		return (input_error("Environement", NULL, 1));
	if (access(argv[1], R_OK) == -1)
		return (input_error("Infile", NULL, 1));
	if (access(argv[argc - 1], F_OK) == 0 && access(argv[argc - 1], W_OK) == -1)
		return (input_error("Outfile", NULL, 1));
	// if (access(argv[argc - 1], F_OK) == 0)
	// 	unlink(argv[argc - 1]);
	fd = open(argv[argc - 1], O_CREAT, S_IRWXU);
	if (fd == -1)
		return (input_error("Outfile", NULL, 1));
	if (iter_pipes(argc, argv, env, paths) == -1)
		return (input_error("Excve", NULL, 0));
	return (0);
}
