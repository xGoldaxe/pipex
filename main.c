/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleveque <pleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/15 16:24:13 by pleveque          #+#    #+#             */
/*   Updated: 2022/01/17 14:47:31 by pleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	read_command_output(int	*pipe_fd, char *output)
{
	char	buffer[1024];
	int		ret;
	int		status;
	int		fd;

	close(pipe_fd[1]);
	unlink(output);
	fd = open(output, O_WRONLY | O_CREAT, S_IRWXU);
	if (fd == -1)
	{
		perror("open");
		return (1);
	}
	printf("\n<---OUTPUT--->\n");
	while ((ret = read(0, buffer, 1024)) != 0)
	{
		buffer[ret] = 0;
		write(fd ,buffer, strlen(buffer));
		write(1 ,buffer, strlen(buffer));
	}
	printf("\n<---//////--->\n");
	close(fd);
	return (0);
}

int	run_command(int	*pipe_fd, char **argv, char **env, int new_entry)
{
	char	**args;
	char	buffer[1024];
	int		readed;

	args = ft_split(argv[0], ' ');
	if (dup2(pipe_fd[1], 1) == -1)
		perror("dup2");
	if (!args)
		perror("split error");
	if (execve(args[0], &args[0], env) == -1)
		perror("execve");
	return (1);
}

int	main(int argc, char **argv, char **env)
{
	pid_t	pid;
	int		pipe_fd[2];
	int		new_entry;
	int		arg_i;
	char	*res[1023];

	if (pipe(pipe_fd) == -1)
    {
		perror("pipe");
		return (1);
    }
	new_entry = open(argv[1], O_RDONLY);
	if (new_entry == -1)
	{
		perror("bad entry");
		return (1);
	}
	arg_i = 2;
	if(dup2(pipe_fd[0], 0) == -1)
		perror("dup2 2/2");
	while (argv[arg_i + 1])
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return (1);
		}
		if (pid == 0)
		{
			if (arg_i == 2)
			{
				if (dup2(new_entry, 0) == -1)
					perror("dup2 2/2");
			}
			printf("run command %s\n", argv[arg_i]);
			run_command(pipe_fd, &argv[arg_i], env, new_entry);
		}
		else
			wait(0);
		++arg_i;
	}
	printf("parent now read!\n");
	read_command_output(pipe_fd, argv[argc - 1]);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return 0;
}