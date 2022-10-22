/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adinari <adinari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 20:42:56 by adinari           #+#    #+#             */
/*   Updated: 2022/10/22 21:35:53 by adinari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

void	argc_err(int argc, int n)
{
	if (argc < n)
	{
		free_2d(&g_pipe.parse.split_envp);
		perror("invalid number of arguments\n");
		exit(1);
	}
}

void	fd_err(int i)
{
	if (i == 1)
		perror("inable to open infile\n");
	else
		perror("dup2 error\n");
	exit(1);
}
