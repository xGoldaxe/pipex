/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleveque <pleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/15 16:24:13 by pleveque          #+#    #+#             */
/*   Updated: 2022/01/18 19:47:58 by pleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
			{
				free_split(p_cmd);
				return (-1);
			}
			pid = fork();
			if (pid == -1)
			{
				free_split(p_cmd);
				return (-1);
			}
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
	if (access(argv[1], R_OK) == -1)
		return (input_error("Infile", NULL, 1));
	if (access(argv[argc - 1], F_OK) == 0 && access(argv[argc - 1], W_OK) == -1)
		return (input_error("Outfile", NULL, 1));
	fd = open(argv[argc - 1], O_CREAT, S_IRWXU);
	if (fd == -1)
		return (input_error("Outfile", NULL, 1));
	paths = get_paths(env);
	if (!paths)
		return (input_error("Environement", NULL, 1));
	if (iter_pipes(argc, argv, env, paths) == -1)
	{
		free_split(paths);
		return (input_error("Excve", NULL, 0));
	}
	free_split(paths);
	return (0);
}
