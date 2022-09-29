#include <unistd.h>
#include <stdio.h>

int main (void)
{
	int fileds[2];
	int pid;
	

	if (pipe(fileds) == -1)
		return(0);
	
	pid = fork();
	
	if (pid == 0)
		printf("I'm in the child\n");
	else if(pid > 0)
		printf("I'm in the parent\n");
	
}
