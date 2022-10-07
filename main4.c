/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adinari <adinari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/02 00:08:24 by adinari           #+#    #+#             */
/*   Updated: 2022/10/05 20:09:18 by adinari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include "pipex.h"
#include "libft/libft.h"
#include <fcntl.h>


//this version takes care of multiple pipes, without using infile or outputting to outfile

char	**ft_split(char const *s, char c);

char *return_correct_path(char **string, char *cmd)
{
	int i;
	char *path;
	
	if (access(cmd, F_OK | X_OK) == 0)
	{
		return (cmd);
	}
	i = 0;
	while(string[i] != 0)
	{
		path = ft_strjoin(string[i], "/");
		path = ft_strjoin(path, cmd);
		if (access(path, F_OK | X_OK) == 0)
			return (path);
		i++;
	}
	// perror("invalid cmd\n");
	// exit(1);
	return (NULL);

}


// ./a.out infile "ls -l" "wc -l" outfile
/*result must be 20, first cmd must be ignored*/
int	main(int argc, char *argv[], char *envp[])
{
	int	i;
	int	fd[2];
	pid_t	pid;
	int	error_code;
	char	**split;

	i = 2;
	error_code = 0;
	while (i <= argc - 2)
	{
		split = ft_split(argv[i], ' ');
		pipe(fd);
		//put infile here, restrict to only happen once at start
		pid = fork();
		//put redir to file here on last loop
		if (pid == 0)
		{
			//happen only if pipe
			
			if (i < argc - 2)
			{
				// write(2, "asdf\n", 5);
				dup2(fd[1], 1);
			}
			//redirect to outfile alwasy on last loop
			close (fd[0]);
			execve(argv[i], split, envp);
			//child
		}
		else
		{
			dup2(fd[0], 0);
			close (fd[1]);
			waitpid(pid, &error_code, 0);
			//parent
		}
		i++;
	}
	// t_pipe	P;
	// char **split1;//cmd1 and parameters
	// char **split2;//cmd2 and parameters
	// char **split_envp;//env split using :
	// char *checker;
	// int		fd[2];
	// int		infile;

	// 	// dup2(outfile, 1);
	// 	// close(outfile);

	// P.read = 0;
	// P.write = 1;


	// if (argc < 5)
	// {
	// 	perror ("missing arguments");
	// 	exit (1);
	// }

	// int i = -1;
	// while (envp[++i])
	// {
	// 	if (!ft_strncmp(envp[i], "PATH=", 5))
	// 		break;
	// }

	// split_envp = ft_split(*(envp + i) + 5, ':');
	// split1 = ft_split(argv[2], ' ');
	// split2 = ft_split(argv[3], ' ');

	// checker = return_correct_path(split_envp,split1[0]);
	// printf("checker %s\n", checker);

	// pipe(fd);

	// int	pid2;
	// int	pid1;

	// pid1 = fork();
	// if (pid1 == 0)
	// {
	// 	infile = open(argv[1], O_RDONLY);
	// 	if (infile == -1)
	// 	{
	// 		perror("Error\n");
	// 		exit(1);
	// 	}
	// 	dup2(infile, 0);
	// 	dup2(fd[1], 1);
	// 	close(fd[0]);
	// 	close(fd[1]);
	// 	close(infile);
	// 	// dprintf(2, "cmd1 %s, args %s , %s\n", checker, split1[0], split1[1]);
	// 	if (execve(checker, split1, envp) == -1)
	// 	{
	// 		perror("command not found");
	// 		exit(1);
	// 	}
	// }
	// else
	// {	
	// 	checker = return_correct_path(split_envp, split2[0]);
	// 	pid2 = fork();
	// 	if (pid2 == 0)
	// 	{

	// 		int	outfile = open(argv[4], O_WRONLY | O_CREAT, 0644);	
	// 		if (outfile == -1)
	// 		{
	// 			perror("failed to create outfile");
	// 			exit(1);
	// 		}

	// 		dup2(fd[0], 0);
	// 		dup2(outfile, 1);
	// 		close(fd[0]);
	// 		close(fd[1]);
	// 		dprintf(2, "cmd2 %s, args %s , %s\n", checker, split2[0], split2[1]);
	// 		if (execve(checker, split2, envp) == -1)
	// 		{
	// 			perror("Command not found");
	// 			exit(1);
	// 		}
	// 	}
	// 	// wait(NULL);

	// 	return (0);
	// }
	// waitpid(pid2, 0, 0);
	// waitpid(pid1, 0, 0);
	// close(fd[0]);
	// close(fd[1]);

}

// PERMISSION DENIED
// FILE NOT FOUND
// COMMAND NOT FOUND
// FILE NOT FOUND FOR SOME COMMANDS (E.Q CAT)
// BAD INPUT (ARGV == 5 FOR MENDATORY AND 6 FOR BONUS)