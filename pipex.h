/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adinari <adinari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 21:20:09 by adinari           #+#    #+#             */
/*   Updated: 2022/10/22 21:32:26 by adinari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include "libft/libft.h"
# include "gnl/get_next_line.h"

typedef struct file
{
	int	infile;
	int	outfile;
	int	tmp;
}				t_file;

typedef struct parse
{
	char	**cmd;
	char	**split_envp;
	char	*path;
}				t_parse;

typedef struct pipe
{
	int		fd[2];
	pid_t	pid;
	t_file	file;
	t_parse	parse;
	int		error_code;
	int		append;
}				t_pipe;

t_pipe	g_pipe;

int		init_here_doc(char *argv[]);
void	child(char *argv[], int argc, int i, char *envp[]);
void	parent(void);
void	init_outfile(char *argv[], int argc);
int		init_infile(char *argv[], int argc);
void	free_2d(char ***to_free);
void	argc_err(int argc, int n);
void	fd_err(int i);

#endif