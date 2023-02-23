#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#define m 2
#define p 2
#define n 2
int matrice1[m][p] = {{1,2},{3,4}};
int matrice2[p][n] = {{1,2},{3,4}};
int prod[m][n];

void* produsMatrice(void *v)
{
  int rez = 0;
  int i = ((int*)v)[0];
  int j = ((int*)v)[1];
  for (int a=0;a<p;a++)
  	rez += matrice1[i][a] * matrice2[a][j];
  prod [i][j] = rez;
  free(v);
  return NULL;
}
int main(int argc, char* argv[])
{
	int index =0;
	pthread_t *thr =(pthread_t*)malloc(m*n*sizeof(pthread_t));
	
	for (int i=0;i<m;i++)
		for (int j=0; j<n;j++)
		{
		void *v = malloc(2);
		((int*)v)[0] = i;
		((int*)v)[1] = j;
		if (pthread_create(&thr[index++], NULL, produsMatrice, v))
		{
		perror("Eroare creare thread!\n");
		return errno;
		}
		}	
	void *result;
	
  	for ( int i=0; i<index ;i++)
  		if(pthread_join(thr[i], &result))
	{
	perror("Eroare finalizare thread!\n");
	return errno;
	}
	for ( int i=0; i<m ;i++)
	{
		for ( int j=0; j<n;j++)
			printf("%d ", prod[i][j]);	
	printf("\n");
	}
  	free(thr);
  return 0;
  }
