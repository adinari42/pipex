/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adinari <adinari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/02 00:08:24 by adinari           #+#    #+#             */
/*   Updated: 2022/10/07 17:37:15 by adinari          ###   ########.fr       */
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
	int	fd[2];
	pid_t	pid;
	int	error_code;
	char	**split;
	char **split_envp;//env split using :
	char *path_check;
	int		infile;
	int	outfile;

	split_envp = envp_parse(envp);
	i = 2;
	error_code = 0;
	while (i <= argc - 2)
	{
		pipe(fd);
		if (i == 2)
		{
			infile = open(argv[1], O_RDONLY);
			if (infile == -1)
			{
				perror("Error:\n");
				exit(1);
			}
			dup2(infile, 0);
			close(infile);
		}
		split = ft_split(argv[i], ' ');
		path_check = return_correct_path(split_envp, split[0]);
		pid = fork();
		if (pid == 0)
		{
			if (i < argc - 2)
				dup2(fd[1], 1);
			if (i == argc - 2)
			{
				outfile = open(argv[argc - 1], O_WRONLY | O_CREAT, 0644);	
				if (outfile == -1)
				{
					perror("failed to create outfile");
					exit(1);
				}
				dup2(outfile, 1);
			}
			close (fd[0]);
			if (execve(path_check, split, envp) == -1)
			{
				perror("command not found");
				exit(1);
			}
		}
		else
		{
			dup2(fd[0], 0);
			close (fd[1]);
			waitpid(pid, &error_code, 0);
		}
		i++;
	}
	close(fd[0]);
	close(fd[1]);
	
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
