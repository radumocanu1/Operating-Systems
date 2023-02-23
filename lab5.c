#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#define dim 4096

int main(int argc,char* argv[]){

    int shm_fd;
    char *shm_name = "ex1";

    shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

    if(shm_fd == -1){
        perror ("Problema la apelul functiei shm_open\n");
        return errno;
    }

    int shm_size = argc * dim;   //setam dimensiunea minima 

    int ftrunc = ftruncate(shm_fd, shm_size);  //definim dimensiunea
    if(ftrunc == -1)
    {
        perror("Problema la definirea dimensiunii\n");
        shm_unlink(shm_name);
        return errno;
    }


    printf("starting parent: %d\n", getpid());
    for(int i = 1; i < argc; ++i){
        pid_t pid = fork();
        if(pid < 0){
            perror("Problema la Fork\n");
            return errno;
        } else if(pid == 0){
            char * shm_ptr = mmap(0, dim, PROT_WRITE, MAP_SHARED, shm_fd, (i - 1) * dim);   

            if(shm_ptr == MAP_FAILED){
                perror("Problema cu incarcarea memoriei partajate in spatiul procesului\n");
                shm_unlink(shm_name);
                return errno;
            }


            int argument = atoi(argv[i]);
            if (sprintf(shm_ptr, "Numarul %d: ", argument)<0 || sprintf(shm_ptr,"%d ", argument)<0)
            	perror("Problema la scrierea caracterelor in buffer\n");
            else
            {
            shm_ptr += sprintf(shm_ptr, "Numarul %d: ", argument);
            shm_ptr += sprintf(shm_ptr,"%d ", argument);
            while(argument > 1){
                if(argument & 1){
                    argument = 3 * argument + 1;
                } else{
                    argument = argument / 2;
                }
                shm_ptr += sprintf(shm_ptr, "%d ", argument);
            }
            shm_ptr += sprintf(shm_ptr,"\n"); //adaugam descompunerea numarului dupa metoda collatz pe o linie a fisierului partajat
            printf("Done child with id = %d, parent id = %d\n", getpid(), getppid());
            }
            munmap(shm_ptr, dim); //nu mai este nevoie de zona de memorie incarcata
            exit(0);
            
        } 
    }
    for(int i = 1; i < argc; ++i){
        wait(NULL);
    }
    for(int i = 1; i < argc; ++i){
        char* shm_ptr = mmap(NULL, dim, PROT_READ, MAP_SHARED, shm_fd, (i-1) * dim); //incarcam memoria partajata in spatiul procesului
        if(shm_ptr == MAP_FAILED){
                perror("Problem with memory map in parent process\n");
                shm_unlink(shm_name);
                return errno;
            }

        printf("%s", shm_ptr); //scriem fiecare linie din fisier la std output
        munmap(shm_ptr, dim);
    }
    printf("Done parent, id = %d\n", getppid());
    shm_unlink(shm_name); //sterge obiectele create cu shm_open
    return 0;
}
