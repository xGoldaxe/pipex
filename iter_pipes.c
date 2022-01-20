/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iter_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleveque <pleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 12:03:44 by pleveque          #+#    #+#             */
/*   Updated: 2022/01/20 12:37:04 by pleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	get_first_pipe(char **argv, char **env, char **paths)
{
	if (ft_strcmp(argv[1], "here_doc") == 0)
		return (limited_stdin(argv));
	else
		return (first_cmd(argv, env, paths));
}

int	run_process_command(int first_pipe, int *new_pipe_fd,
char **p_cmd, char **env)
{
	pid_t	pid;

	if (fork_store(&pid) == -1)
		return (-1);
	else if (pid == 0)
		return (run_command(first_pipe, new_pipe_fd, p_cmd, env));
	return (0);
}

int	iter_pipes(int argc, char **argv, char **env, char **paths)
{
	int		first_pipe;
	int		new_pipe_fd[2];
	int		arg_i;
	char	**p_cmd;

	first_pipe = get_first_pipe(argv, env, paths);
	arg_i = 2;
	while (argv[++arg_i + 1])
	{
		p_cmd = parse_cmd(argv[arg_i], paths);
		if (pipe(new_pipe_fd) == -1 || first_pipe == -1)
			return (free_split_int(p_cmd));
		if (p_cmd)
		{
			if (run_process_command(first_pipe, new_pipe_fd, p_cmd, env) == -1)
				return (free_split_int(p_cmd));
		}
		if (dup2(new_pipe_fd[0], first_pipe) == -1)
			return (free_split_int(p_cmd));
		close_pipe(new_pipe_fd);
		free_split(p_cmd);
	}
	return (write_command_output(first_pipe, argv[argc - 1],
			ft_strcmp(argv[1], "here_doc") == 0));
}
