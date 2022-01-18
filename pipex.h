/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleveque <pleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/15 16:24:21 by pleveque          #+#    #+#             */
/*   Updated: 2022/01/18 19:13:50 by pleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <string.h>
# include <fcntl.h>
# include <sys/stat.h>
# include "get_next_line.h"

/* UTILS */
char	*ft_strjoin(char const *s1, char const *s2);
int		ft_strlen(char const *str);
char	**ft_split(char const *s, char c);
void	*free_split(char **splited);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_strclen(char *str, char c);
int		ft_biggest(int a, int b);

/* MAIN */
int		first_cmd(char **argv, char **env, char **paths);
int		run_command(int entry_pipe, int	*pipe_fd, char **argv, char **env);
char	**parse_cmd(char *command, char **paths);
int		input_error(char *error_type, char	*precision, int type);
int		limited_stdin(char **argv);
int		write_command_output(int pipe_fd, char *output, int add_to);

#endif