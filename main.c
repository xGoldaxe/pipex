/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleveque <pleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/15 16:24:13 by pleveque          #+#    #+#             */
/*   Updated: 2022/01/20 12:35:49 by pleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
		if (type == 1)
		{
			write(2, "\033[1;31mInvalid arguments\033[0m\n", 29);
			printf("Try: ./pipex file1 <cmd1> <cmd2> <...> file2");
			printf("\n     ./pipex here_doc <LIMITER> <cmd> <cmd1> <...> file");
		}
		else
		{
			printf("\033[1;31m%s\033[0m", error_type);
			if (type == 2)
				printf("Invalid command: \033[1;35m%s\n\033[0m", precision);
			if (type == 3)
				printf("Invalid file: \033[1;35m%s\n\033[0m", precision);
		}
	}
	free(error);
	return (1);
}

int	main(int argc, char **argv, char **env)
{
	char	**paths;
	int		fd;

	if (argc < 5)
		return (input_error("Arguments", NULL, 1));
	if (ft_strcmp("here_doc", argv[1]) != 0 && access(argv[1], R_OK) == -1)
		return (input_error("Infile", argv[1], 3));
	if (access(argv[argc - 1], F_OK) == 0
		&& access(argv[argc - 1], W_OK) == -1)
		return (input_error("Outfile", argv[argc - 1], 3));
	fd = open(argv[argc - 1], O_CREAT, S_IRWXU);
	if (fd == -1)
		return (input_error("Outfile", argv[argc - 1], 3));
	paths = get_paths(env);
	if (!paths)
		return (input_error("Environement", NULL, 4));
	if (iter_pipes(argc, argv, env, paths) == -1)
	{
		free_split(paths);
		return (input_error("Excve", NULL, 0));
	}
	free_split(paths);
	return (0);
}
