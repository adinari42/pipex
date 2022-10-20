/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adinari <adinari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 17:32:59 by adinari           #+#    #+#             */
/*   Updated: 2022/10/20 21:21:07 by adinari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include "pipex.h"
#include "libft/libft.h"
#include <fcntl.h>

//this version takes care of multiple pipes with direct path or single command.
char	**ft_split(char const *s, char c);

char	*ret_path(char **string, char *cmd)
{
	int		i;
	char	*path;
	char	*path_temp;

	i = 0;
	while (string[i] != 0)
	{
		path_temp = ft_strjoin(string[i], "/");
		path = ft_strjoin(path_temp, cmd);
		free(path_temp);
		if (access(path, F_OK | X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	if (access(cmd, F_OK | X_OK) == 0)
		return (ft_strdup(cmd));
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
			break ;
	}
	envp_parse = ft_split(*(envp + j) + 5, ':');
	return (envp_parse);
}

void	free_2d(char ***to_free)
{
	size_t	i;

	i = 0;
	if (*to_free == NULL)
		return ;
	while ((*to_free)[i] != NULL)
	{
		free((*to_free)[i]);
		++i;
	}
	free(*to_free);
	*to_free = NULL;
}

void	init_path(char **argv, int i)
{
	g_pipe.parse.cmd = ft_split(argv[i], ' ');
	g_pipe.parse.path = ret_path(g_pipe.parse.split_envp,
			g_pipe.parse.cmd[0]);
}

void	free_and_close(void)
{
	free_2d(&g_pipe.parse.split_envp);
	close(g_pipe.fd[0]);
	close(g_pipe.fd[1]);
	unlink("tmp");
}

int	main(int argc, char *argv[], char *envp[])
{
	int	i;

	g_pipe.parse.split_envp = envp_parse(envp);
	i = 0;
	g_pipe.error_code = 0;
	while (i <= argc - 2)
	{
		pipe(g_pipe.fd);
		if (i == 0)
			i = init_infile(argv, argc);
		init_path(argv, i);
		g_pipe.pid = fork();
		if (g_pipe.pid == -1)
		{
			perror("Fork failure\n");
			exit(1);
		}
		if (g_pipe.pid == 0)
			child(argv, argc, i, envp);
		else
			parent();
		i++;
		free(g_pipe.parse.path);
		free_2d(&g_pipe.parse.cmd);
	}
	
	free_and_close();
	// system("leaks pipex");
}
