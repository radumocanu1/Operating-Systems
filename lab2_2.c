#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
int main(int argc, char* argv[]){
	if (argc != 3)
	{
	printf ("The program needs exactly 2 args, try again :) \n");
	return 0;}
	char* fileToCopyFrom = argv[1];
	char* fileToCopyTo = argv[2];
	
	int openFile = open(fileToCopyFrom, O_RDONLY);
	if (openFile == -1)
	{
	printf("Eroare la deschiderea fisierului de copiat  %d\n", errno);
	return errno;
	}
	int openDest = open (fileToCopyTo, O_CREAT | O_WRONLY, S_IRWXU);
	if (openDest == -1)
	{
	printf("Eroare  %d\n", errno);
	return errno;
	}
	ssize_t reader = -1;
	char buffer[1024];
	while (reader = read(openFile, buffer, 1024))
	{
	
		int written = 0;
		int writer;
		while ( writer = write(openDest, buffer, reader-written))
			written = written + writer;
			if (written == -1)
		{
			printf("Eroare  %d\n", errno);
			return errno;
		}
	}
	if (reader == -1)
		{
			printf("Eroare  %d\n", errno);
			return errno;
		}
	close (openFile);
	close (openDest);
	

return 0;
}
