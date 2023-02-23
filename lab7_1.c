#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#define available_resources 5
int nr_resources = available_resources;
pthread_mutex_t mtx;
int getRandomNumber()
{
int random = rand() % nr_resources;
 return random; 
}
 

void *thread_func(void *argc){

	int randomNumber = getRandomNumber();
	if (decrease_count(randomNumber) == -1)
	{
	printf("Not enough available resources ( needed %d, but got %d)", randomNumber, nr_resources) ;
	}
	increase_count(randomNumber);
		
		
		
}
int increase_count (int count){
   pthread_mutex_lock(&mtx);
   nr_resources += count;
   printf("Released %d resources %d remaining \n", count, nr_resources);
   pthread_mutex_unlock(&mtx);
   return 0;
}
int decrease_count ( int count )
{
	pthread_mutex_lock(&mtx);
	if (nr_resources < count)
	{
	pthread_mutex_unlock(&mtx);
	return -1;
	}
	else
	{
	nr_resources -=count;
	printf("Got %d resources %d remaining \n", count, nr_resources);
	}
	pthread_mutex_unlock(&mtx);
	
	return 0;
}
int main(int argc, char *argv[]){

	printf("Available resources = %d \n", nr_resources);
	if (pthread_mutex_init(&mtx, NULL)){
	  perror("Eroare crearea mutex");
	  return errno;
	}
	int nr_threads =strtol(	argv[1], NULL, 10);
	pthread_t *thr = (pthread_t*)malloc(nr_threads*sizeof(pthread_t));
	
	for (int i=0; i<nr_threads; i++)
	{
		if (pthread_create(thr+i, NULL, thread_func, NULL)){
		perror("Eroare creare thread");
		return errno;
		}
	}
	
	void *result;
	for (int i=0; i<nr_threads; i++)
	{
	if (pthread_join(thr[i], &result)){
		perror("eroare join");
		return errno;
	 }
	}
	free(thr);
	pthread_mutex_destroy(&mtx);


	return 0;
}
