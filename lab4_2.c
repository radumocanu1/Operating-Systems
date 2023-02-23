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
	int number = atoi(argv[1]);
	pid_t newPid = fork(); 
	if (newPid < 0)
	{
	printf("eroare creare proces");
	return errno;
	}
	if (newPid == 0)
	{
	printf("%d: ", number);
	collatz(number);
	}
	else
	{
	wait(NULL);
	printf("\nChild %d finished\n", newPid);
	}
	return 0;
	
}
