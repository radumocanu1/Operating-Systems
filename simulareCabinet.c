#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define numar_doctori 3
#define numar_max_pacienti 30
#define perioada_maxima_consultatie 6
#define max_timp_sosire_pacient_nou 4
#define max_timp_in_consultatie 10


int nr_clienti;
int nr_doctori_disponibili = numar_doctori;
int doctor_disponibil[numar_doctori];
pthread_mutex_t mtx;

struct doctor{
int index;
char *nume;
char *prenume;
};
struct doctor doctori[numar_doctori];

int getRandomNumber(int index)
{
int random = rand() % index;
 return random; 
}


int verificare_doctori_disponibili(){
	pthread_mutex_lock(&mtx);
	if (nr_doctori_disponibili == 0 )
	{
	pthread_mutex_unlock(&mtx);
	return 0;
	}
	nr_doctori_disponibili -= 1;
	pthread_mutex_unlock(&mtx);
	return 1;
	
		
}
int get_doctor_disponibil()
{
	pthread_mutex_lock(&mtx);
	for (int i=0;i<=numar_doctori;i++)
		if (doctor_disponibil[i])
			{
				doctor_disponibil[i] = 0;
				pthread_mutex_unlock(&mtx);
				return i;
			}
	pthread_mutex_unlock(&mtx);		
	return -1;
}
int incepe_consultatie (int index_doctor)
{
	//timp consultatie 
	printf("Doctorul %s  %s a intrat in consultatie\n\n", doctori[index_doctor].nume, doctori[index_doctor].prenume);
	int timp_consultatie = getRandomNumber(max_timp_in_consultatie) + 1;
	sleep(timp_consultatie);
	//doctorul devine disponibil 
	printf("Doctorul %s  %s a iesit din consultatie\n", doctori[index_doctor].nume, doctori[index_doctor].prenume);
	doctor_disponibil[index_doctor] = 1; 
	nr_doctori_disponibili += 1;
	return timp_consultatie;
}

void *thread_func(void* input){

	int index_doctor_disponibil;
	int prima_incercare = 1; 
	int* index = (int*)input;
	printf("Pacientul %d a ajuns la spital\n", index);
	clock_t timp_asteptare = clock();
	//pacientul incearca sa gaseasca un doctor disponibil odata la 3 secunde 
	while (1)
	{
		if (!prima_incercare)
		{
		sleep(3);
		}
		else
		{
		prima_incercare = 0;
		}
		if (verificare_doctori_disponibili() != 0)
		{
			index_doctor_disponibil	= get_doctor_disponibil();
			if (index_doctor_disponibil == -1)
			{
				printf("Ups, s-a produs o eroare\n");

			}
			timp_asteptare = clock() - timp_asteptare;
			int timp_consultatie = incepe_consultatie(index_doctor_disponibil);
			printf("Pacientul %d a stat %d secunde in consultatie si a asteptat %f secunde\n", index, timp_consultatie, (float)timp_asteptare/CLOCKS_PER_SEC);
			break;	
		}
			
	}
	
		
}


void init_doctori(){
	doctori[0].index = 0;  
	doctori[0].nume = (char*)malloc(sizeof(char)*(strlen("Ionescu") + 1));
	doctori[0].nume = "Ionescu";
	doctori[0].prenume = (char*)malloc(sizeof(char)*(strlen("Andrei") + 1));
	doctori[0].prenume = "Andrei";
	doctori[1].index = 1;
	doctori[1].nume = (char*)malloc(sizeof(char)*(strlen("Popescu") + 1));
	doctori[1].prenume = (char*)malloc(sizeof(char)*(strlen("Mircea") + 1));
	doctori[1].nume = "Popescu";
	doctori[1].prenume = "Mircea";
	doctori[2].index = 2;
	doctori[2].nume = (char*)malloc(sizeof(char)*(strlen("Anton") + 1));
	doctori[2].prenume = (char*)malloc(sizeof(char)*(strlen("Marian") + 1));
	doctori[2].nume = "Anton";
	doctori[2].prenume = "Marian";
	
	for (int i=0;i<=numar_doctori;i++)
		doctor_disponibil[i] = 1;
		 
	
}

int main(int argc, char *argv[]){
	
	int index_pacient = 0;
	init_doctori();
	int perioada_de_timp =strtol(argv[1], NULL, 10);
	
	if (pthread_mutex_init(&mtx, NULL)){
	  perror("Eroare crearea mutex");
	  return errno;
	}
	
	pthread_t *thr = (pthread_t*)malloc(numar_max_pacienti*sizeof(pthread_t));
	
	int perioada_exec = 0 ;

	while (perioada_exec < perioada_de_timp)
	{
		index_pacient += 1;
		int randomNum = getRandomNumber(max_timp_sosire_pacient_nou);
		
			
		sleep(randomNum);
		perioada_exec += randomNum;
		if (pthread_create(thr + index_pacient - 1, NULL, thread_func, (void*)index_pacient)){
		perror("Eroare creare thread");
		return errno;
		}
		
		
		
	}
	
	
	void *result;
	for (int i=0; i<index_pacient; i++)
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
