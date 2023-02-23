#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

void* hello(void *v)
{
  char *who = (char*)v;
  size_t length = strlen(who);
  char* reverseString = (char*)malloc((length+1)* sizeof(char));
  reverseString[length] = '\0';
  int i;
  for (i=0;i<length;i++)
    reverseString[i] = who[length - 1 - i];
  printf("%s\n", reverseString);
  free(reverseString);
  return NULL;
}
int main(int argc, char* argv[])
{
	pthread_t thr;
	if (argc!=2)
	{
	  printf("Numar invalid argumente \n");
	  return EINVAL;
	}
	if (pthread_create(&thr, NULL, hello, argv[1]))
	{
	perror("Eroare creare thread!\n");
	return errno;
	}
	
	void *result;
	if(pthread_join(thr, &result))
	{
	perror("Eroare finalizare thread!\n");
	return errno;
	}
	else
	{
	printf(result);
	}
  
  return 0;
  }
