#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#define numberOfThreads 10
int threadsPos[numberOfThreads];
int currentThreadNumber = 0;

pthread_mutex_t mtx;
sem_t sem;

void barrier_point(int tid)
{
	pthread_mutex_lock(&mtx);
	currentThreadNumber++;
	
	if (currentThreadNumber == numberOfThreads)
	{
		pthread_mutex_unlock(&mtx);
		if (sem_post(&sem))
		{
			printf("Error: %d\n", errno);
			return;
		}
		return;
	}
	
	pthread_mutex_unlock(&mtx);
	printf(" thread %d blocked\n", tid);
	if (sem_wait(&sem))
	{
		printf("Error: %d\n", errno);
		return;
	}
	
	if (sem_post(&sem))
	{
		printf("Error: %d\n", errno);
		return;
	}
	printf(" thread %d unlocked\n", tid);
	return;
}


void* thread_func(void *arg)
{


	
	int *tid = (int*)arg;
	barrier_point(*tid);
	return NULL;
}

int main(int argc, char* argv[])
{
	
	if (pthread_mutex_init(&mtx, NULL))
	{
		perror("Eroare creare mutex!\n");
		return errno;
	}
	
	
	
	if (sem_init(&sem, 0, 0))
	{
		printf("Eroare initializare semafor!\n");
		return errno;
	}
	

	printf("No of threads: %d\n", numberOfThreads);
	pthread_t *thr = (pthread_t*)malloc(numberOfThreads*sizeof(pthread_t));
	
	for (int i=0; i<numberOfThreads; i++)
	{
		threadsPos[i] = i;
		if (pthread_create(thr+i, NULL, thread_func, &threadsPos[i])){
		perror("Eroare creare thread");
		return errno;
		}
	}
	
	void *result;
	for (int i=0; i<numberOfThreads; i++)
	{
	if (pthread_join(thr[i], &result)){
		perror("eroare join");
		return errno;
	 }
	}
	free(thr);
	sem_destroy(&sem);
	pthread_mutex_destroy(&mtx);
	return 0;

}
