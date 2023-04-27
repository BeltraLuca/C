#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void){

    printf("il processo attuale %d\n",getpid());  
    int num;
    if(wait(&num)<0){
        printf("impossibile far attendere un figlio inesistente");
        exit(-1);
    }
    exit(0);
}