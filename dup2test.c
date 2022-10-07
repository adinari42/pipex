
#include <stdio.h>
#include <unistd.h>
#include "pipex.h"
#include "libft/libft.h"
#include <fcntl.h>

char	**ft_split(char const *s, char c);

char *return_correct_path(char **string, char *cmd)
{
	int i;
	char *path;
	
	if (access(cmd, R_OK) == 0)
		return (cmd);
	i = 0;
	while(string[i] != 0)
	{
		path = ft_strjoin(string[i], "/");
		path = ft_strjoin(path, cmd);
		if (access(path, R_OK) == 0)
			return (path);
		i++;
	}
	return (NULL);

}


// ./a.out infile "ls -l" "wc -l" outfile
/*result must be 20, first cmd must be ignored*/
int	main(int argc, char *argv[], char *envp[])
{
	t_pipe	P;
	char **split1;//cmd1 and parameters
	char **split2;//cmd2 and parameters
	char **split_envp;//env split using :
	char *checker;
	int		fd[2];
	int		infile;

	int	outfile = open(argv[4], O_WRONLY | O_CREAT, 0777);
	dup2(outfile, 1);
	close(outfile);
	infile = open(argv[1], O_RDONLY);
	dup2(infile, 0);
	close(infile);

	P.read = 0;
	P.write = 1;


	if (argc < 5)
	{
		perror ("missing arguments");
		exit (1);
	}

	int i = -1;
	while (envp[++i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
			break;
	}

	split_envp = ft_split(*(envp + i) + 5, ':');
	split1 = ft_split(argv[2], ' ');
	split2 = ft_split(argv[3], ' ');

	checker = return_correct_path(split_envp,split1[0]);



	pipe(fd);

	int	pid2;
	int	pid1;
	
	pid1 = fork();


		if (pid1 == 0)
		{
			close(fd[P.read]);
			dup2(infile, 0);
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
