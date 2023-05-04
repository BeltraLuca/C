#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char** argv){

    if(argc!=2){
        printf("ERRORE, inserire un parametro\n");
        exit(-1);
    }
    
    int N;
    if((N=atoi(argv[1]))==0){
        printf("ERRORE inserire un numero come primo parametro\n");
        exit(-2);
    }

    if(N >90000 || N <0){
        printf("ERRORE il parametro deve essere minore di 255 e maggiore di 0\n");
        exit(-3);
    }

    printf("il processo attuale è %d e il numero riportato è uguale a %d\n", getpid(),N);

    int pid;
    for(int i=0;i<N;i++){
        
        if((pid=fork())<0){
            printf("ERRORE non mi crea il figlio");
            exit(-4);
        }

        if(pid==0){
            printf("il processo figlio ha pid= %d e sono il processo numero %d\n",getpid(),i );
            exit(i);
        }
    }

    for(int i=0;i<N;i++){
        int status;
        if ((pid=wait (&status)) < 0)
        { printf("Errore in wait\n");
            exit(2);
        }
        if ((status & 0xFF) != 0)
            printf("Errore in status\n");
        else
        { int exit_s = status >> 8;
        /* selezione degli 8 bit piu’ significativi */
            exit_s &= 0xFF;
        printf("Per il figlio %d lo stato di EXIT e %d\n",pid, exit_s);
        }
    }
    exit(1);

}