/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adinari <adinari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 21:20:09 by adinari           #+#    #+#             */
/*   Updated: 2022/10/16 20:46:49 by adinari          ###   ########.fr       */
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
	char	**cmd;//for char **split
	char	**split_envp;
	char *path_check;
}				t_parse;

typedef struct pipe
{
	int		fd[2];
	pid_t	pid;
	t_file	file;
	t_parse	parse;
	int		error_code;
}				t_pipe;

t_pipe g_pipe;


#endif