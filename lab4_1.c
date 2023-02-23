#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc,char* argv[])
{
	pid_t newPid = fork(); 
	if (newPid < 0)
	{
	printf("eroare creare proces");
	return errno;
	}
	if (newPid == 0)
	{
	char *operation[] = {"ls", NULL};
	execve("/bin/ls", operation, NULL);
	}
	else
	{
	printf("Parent PID = %d, Child pid = %d\n", getpid(), newPid);
	wait(NULL);
	printf("Child %d finished\n", newPid);
	}
	return 0;
	
}
