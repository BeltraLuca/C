#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int mia_random(int n)
{
int casuale;
casuale = rand() % n;
return casuale;
}

int main(void){
    printf("il processo padre inizia\n");
    int pid=0;
    if((pid=fork())<0){
        printf("ERRORE nella creazione del processo figlio");
        exit(-1);
    }
    if(pid==0){        //codice eseguito dal figlio
        printf("\nil processo figlio inizia\n");
        srand(time(NULL));
        printf("processo figlio %d con padre %d\n",getpid(),getppid());
        printf("il processo figlio termina\n\n");
        exit(mia_random(100));
    }else{                      //codice eseguito dal padre
        int num=0;

        if (wait(&num) < 0)
        { printf("Errore in wait\n");
            exit(2);
        }
        if ((num & 0xFF) != 0){
            printf("Errore in status\n");
        }
        printf("il processo attuale %d ha creato un processo figlio %d, il valore di ritorno %d\n", getpid(),pid,num>>8 );
        printf("il processo padre termina\n");
        exit(0);
    }


}

