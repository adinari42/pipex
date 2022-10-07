typedef  struct s_commands
{
	char *infile;
	char *outfile;
	char  *cmd;
	char **args;
	struct s_commands *next;	
} 


int main()
{	
	//parse the argv
	infile = argv[1];
	cmd1 = argv[2];
	cmd2 = argv[3];
	outfile = argv[4];



}

int execution ()
{
	int fd[2];

	int infile = open("infile", O_RDONLY);
	int outfile = open("outfile", O_WRONLY);


	if(pipe(fd) == -1)
	{

	}

	int pid = fork();
	
	if (cmd.next != null)
	{
		dup2(infile, 0);
		close(infile);
		dup2(fd[0], 1);
		close(fd[0]);
	}
	else
	{
		dup2(fd[1], 0);
		close(fd[1]);
		dup2(outfile, 1);
		close(outfile);
	}
	if (pid == 0)
	{
		execve()
	}

	return (exist status)
}