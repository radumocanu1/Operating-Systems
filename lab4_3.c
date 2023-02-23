#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
void collatz(int number){
	printf("%d ", number);
	if (number == 1)
	   return;
	if (number % 2 == 0)
		collatz(number/2);
	else
		collatz(number*3+1);
	   
}
int main(int argc,char* argv[])
{
	int n = argc-1;
	int *numbers = malloc(sizeof(int) * argc);
	for (int i=0; i<n; i++)
	{
		numbers[i] = atoi(argv[i+1]);
	}
	printf("Starting parent %d \n", getpid());
	for (int i=0; i<n ;i++)
	{
		pid_t newPid = fork();
	
		if (newPid <0)
		{
			printf("eroare creare proces");
			return errno;
		}
		if (newPid == 0)
		{
			printf("%d: ", numbers[i]);
			collatz(numbers[i]);
			printf("\n");
			printf("Done Parent %d Me %d \n", getppid(), getpid());
			exit(0);
	
		}
	
	}
	//for (int i=0 ; i<n; i++)
		wait(NULL);
	printf("Done Parent %d Me %d \n", getppid(), getpid());
	return 0;
	
}
