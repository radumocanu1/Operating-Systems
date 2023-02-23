#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
int x;
pthread_mutex_t mtx;
void *thread_func(void *argc){
	for (int i=0;i< 10000; i++){
	
		pthread_mutex_lock(&mtx);
		x = x+1;
		pthread_mutex_unlock(&mtx);
		}
}
int main(int argc, char *argv[]){
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
	printf ("%d", x);

	return 0;
}
