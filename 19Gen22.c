#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <ctype.h>
#define PERM 0644

typedef int pipe_t[2];

int main(int argc, char** argv){

    if(argc<3){
        perror("Errore nell'inserimento dei parametri\n");
        exit(-1);
    }

    int C,i=0,N=argc-2;
    pipe_t *piped=calloc(N,sizeof(pipe_t));
    int *pid=calloc(N*2,sizeof(int));

    if(piped==NULL || pid==NULL){
        perror("errore nell'allocazione\n");
        exit(-3);
    }

    for(int j=0;j<N;j++){
        if((pipe(piped[j]))<0){
            perror("errore nella creazione della pipe\n");
        }
    }

    if((C=atoi(argv[argc-1]))>0){
        if(C%2==0){
            perro("il numero non è dispari\n");
            exit(-2);
        }
    }else{
        perror("l'ultimo parametro non è positivo\n");
        exit(-2);
    }


    for(int j=0;j<N;j++){
        
    }
}