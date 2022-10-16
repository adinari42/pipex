/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adinari <adinari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/02 00:08:24 by adinari           #+#    #+#             */
/*   Updated: 2022/10/16 20:49:06 by adinari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include "pipex.h"
#include "libft/libft.h"
#include <fcntl.h>

//this version takes care of multiple pipes with direct path or single command, without using infile or outputting to outfile

char	**ft_split(char const *s, char c);

char	*return_correct_path(char **string, char *cmd)
{
	int		i;
	char	*path;

	i = 0;
	while(string[i] != 0)
	{
		path = ft_strjoin(string[i], "/");
		path = ft_strjoin(path, cmd);
		if (access(path, F_OK | X_OK) == 0)
			return (path);
		i++;
	}
	if (access(cmd, F_OK | X_OK) == 0)
		return (cmd);
	return (NULL);
}

char	**envp_parse(char **envp)
{
	int		j;
	char	**envp_parse;

	j = -1;
	while (envp[++j])
	{
		if (!ft_strncmp(envp[j], "PATH=", 5))
			break;
	}
	envp_parse = ft_split(*(envp + j) + 5, ':');
	return (envp_parse);
}



int	main(int argc, char *argv[], char *envp[])
{
	int	i;
	int	append;

	append = 0;
	g_pipe.parse.split_envp = envp_parse(envp);
	i = 0;
	g_pipe.error_code = 0;

	while (i <= argc - 2)
	{
		pipe(g_pipe.fd);
		if (i == 0)//this will be skipped when using here_doc, i = 3
		{	if (ft_strncmp(argv[1], "here_doc", ft_strlen(argv[1])) == 0)
			{
				char	*str;

				g_pipe.file.infile = open("tmp", O_WRONLY | O_CREAT, 0644);
				g_pipe.file.tmp = open("tmp", O_RDONLY | O_CREAT);
				str = get_next_line(0);
				while (1)
				{	
					if (ft_strncmp(argv[2], str, ft_strlen(str) - 1) == 0)//need to check why strlen returns bigger number
						break;
					ft_putstr_fd(str, g_pipe.file.infile);
					str = get_next_line(0);
				}
				
				i = 3;
				append = 1;
				dup2(g_pipe.file.tmp, 0);
				close(g_pipe.file.infile);
				close(g_pipe.file.tmp);
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
				i = 2;
			}

		}
		g_pipe.parse.cmd = ft_split(argv[i], ' ');
		g_pipe.parse.path_check = return_correct_path(g_pipe.parse.split_envp, g_pipe.parse.cmd[0]);
		g_pipe.pid = fork();
		if (g_pipe.pid == 0)
		{
			if (i < argc - 2)
				dup2(g_pipe.fd[1], 1);
			if (i == argc - 2)
			{
				if (append == 0)
					g_pipe.file.outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
				else
					g_pipe.file.outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
				if (g_pipe.file.outfile == -1)
				{
					perror("failed to create outfile");
					exit(1);
				}
				dup2(g_pipe.file.outfile, 1);
			}
			close (g_pipe.fd[0]);
			if (execve(g_pipe.parse.path_check, g_pipe.parse.cmd, envp) == -1)
			{
				
				perror("command not found");
				exit(1);
			}
		}
		else
		{
			dup2(g_pipe.fd[0], 0);
			close (g_pipe.fd[1]);
			waitpid(g_pipe.pid, &g_pipe.error_code, 0);
		}
		i++;
	}
	close(g_pipe.fd[0]);
	close(g_pipe.fd[1]);
	unlink("tmp");
}

// PERMISSION DENIED
// FILE NOT FOUND
// COMMAND NOT FOUND
// FILE NOT FOUND FOR SOME COMMANDS (E.Q CAT)
// BAD INPUT (ARGV == 5 FOR MENDATORY AND 6 FOR BONUS)



//Line79: create infile here, restrict to only happen once at start
//LINE87: read from infile instead of standard input (make a copy of it that replaces stdin)
//LINE96: //redirects whatever was written on stdout to stdin(write) of next cmd 
//LINE97: redirect to outfile alwasy on last loop
//LINE116: redirect output of cmd to stdin of next cmd
