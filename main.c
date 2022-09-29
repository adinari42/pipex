/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adinari <adinari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 21:19:54 by adinari           #+#    #+#             */
/*   Updated: 2022/09/29 20:15:00 by adinari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include "pipex.h"
#include "libft/libft.h"

char	**ft_split(char const *s, char c);

char *return_correct_path(char **string, char *cmd)
{
	int i;
	char *path;
	
	if (access(cmd, R_OK) == 0)
	{
		printf("string path >> %s\n", cmd);
		return (cmd);
	}
	i = 0;
	while(string[i] != 0)
	{
		path = ft_strjoin(string[i], "/");
		path = ft_strjoin(path, cmd);
		printf("string path = %s\n", path);
		if (access(path, R_OK) == 0)
		{
			printf("string path >> %s\n", path);
			return (path);
		}
		i++;
	}
	return (NULL);

}
int	main(int argc, char *argv[], char *envp[])
{
	t_pipe	P;
	char **split1;//cmd1 and parameters
	char **split2;//cmd2 and parameters
	char **split_envp;//env split using :
	char *checker;
	int		fd[2];

	P.read = 0;
	P.write = 1;

	int i = -1;
	while (envp[++i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
		{
			printf("%s... %d \n", envp[i], i);
			break;
		}
	}
	printf(" i here = %d\n", i);
	printf("env here : %s\n", *(envp + i) + 5);
	split_envp = ft_split(*(envp + i) + 5, ':');
	printf("split_envp[0] = %s\n", split_envp[0]);
	split1 = ft_split(argv[1], ' ');
	split2 = ft_split(argv[2], ' ');
printf("split1[0] = %s\n", split1[0]);//
printf("split2[0] = %s\n", split2[0]);//
	checker = return_correct_path(split_envp,split1[0]);
	printf("checker = %s\n", checker);


	pipe(fd);

	int	pid2;
	int	pid1;
	
	pid1 = fork();

	if (pid1 == 0)
	{
		close(fd[P.read]);
		dup2(fd[P.write], P.write);
		execve(checker, split1, envp);
	}
	else
	{
		checker = return_correct_path(split_envp, split2[0]);
		pid2 = fork();
		if (pid2 == 0)
		{
			close(fd[P.write]);
			dup2(fd[P.read], P.read);
			execve(checker, split2, envp);
		}
		waitpid(pid1, 0, 0);
		close(fd[1]);
		waitpid(pid2, 0, 0);
		return (0);
	}
}
