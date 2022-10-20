/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adinari <adinari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/16 23:18:53 by adinari           #+#    #+#             */
/*   Updated: 2022/10/20 20:57:31 by adinari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	init_here_doc(char *argv[])
{
	char	*str;

	g_pipe.file.infile = open("tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (g_pipe.file.infile == -1)
	{
		perror("inable to open infile\n");
		exit(0);
	}
	g_pipe.file.tmp = open("tmp", O_RDONLY | O_CREAT);
	if (g_pipe.file.infile == -1 || g_pipe.file.tmp == -1)
	{
		perror("Error: unable to open file\n");
		exit(0);
	}
	str = get_next_line(0);
	while (1)
	{
		if (ft_strncmp(argv[2], str, ft_strlen(str) - 1) == 0)
			break ;
		ft_putstr_fd(str, g_pipe.file.infile);
		free(str);
		str = get_next_line(0);
	}
	free(str);
	g_pipe.append = 1;
	if (dup2(g_pipe.file.tmp, 0) == -1)
	{
		perror("dup2 error\n");
		exit(0);
	}
	close(g_pipe.file.infile);
	close(g_pipe.file.tmp);
	return (3);
}

void	argc_err(int argc, int n)
{
	if (argc < n)
	{
		free_2d(&g_pipe.parse.split_envp);
		perror("invalid number of arguments\n");
		exit(1);
	}
}

int	init_infile(char *argv[], int argc)
{
	argc_err(argc, 5);
	g_pipe.append = 0;
	if (ft_strncmp(argv[1], "here_doc", ft_strlen(argv[1])) == 0)
	{
		argc_err(argc, 6);
		return (init_here_doc(argv));
	}
	else
	{
		g_pipe.file.infile = open(argv[1], O_RDONLY);
		if (g_pipe.file.infile == -1)
		{
			perror("Error:\n");
			exit(1);
		}
		dup2(g_pipe.file.infile, 0);
		close(g_pipe.file.infile);
		return (2);
	}
}

void	init_outfile(char *argv[], int argc)
{
	if (g_pipe.append == 0)
		g_pipe.file.outfile = open(argv[argc - 1],
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		g_pipe.file.outfile = open(argv[argc - 1],
				O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (g_pipe.file.outfile == -1)
	{
		perror("failed to create outfile");
		exit(1);
	}
	dup2(g_pipe.file.outfile, 1);
}

void	child(char *argv[], int argc, int i, char *envp[])
{
	if (i < argc - 2)
		dup2(g_pipe.fd[1], 1);
	if (i == argc - 2)
		init_outfile(argv, argc);
	close (g_pipe.fd[0]);
	if (execve(g_pipe.parse.path, g_pipe.parse.cmd, envp) == -1)
	{
		perror("command not found");
		exit(1);
	}
}

// int timeout(int secs)
// {
// 	long start = gettimeofday();

// 	while(gettimeofday() - start < 5)
// 	{
// 		if (waitpid(g_pipe.pid, &g_pipe.error_code, WNOHANG) == 0)
// 			return 0;
// 	}
// )
	
// 	return 1;
// }


void	parent(void)
{
	dup2(g_pipe.fd[0], 0);
	close (g_pipe.fd[1]);
	waitpid(g_pipe.pid, &g_pipe.error_code, 0);
}
