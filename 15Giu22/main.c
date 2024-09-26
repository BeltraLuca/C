#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

typedef int pipe_t[2];       //definizione di un nuovo tipo che definisce una pipe

int main(int argc, char** argv){
    
    //controllo che il numero di parametri passati sia >=3  N=2 file + file chiamato
    if(argc<3){
        perror("errore nel numero di parametri\n");
        exit(-1);
    }

    int N=argc-1;                               //numero file/figli
    int pid;                                    //variabile che mantiene il pid ritornato dalla fork
    pipe_t *piped=calloc(N,sizeof(pipe_t));   //Allocazione N+1 pipe
    pipe_t *pipedres=calloc(N,sizeof(pipe_t));

    //creazione delle N Pipe
    for(int i=0;i<N;i++){
        if(pipe(piped[i])!=0){
            perror("errore nella creazione della pipe\n");
            exit(-2);
        }

        if(pipe(pipedres[i])!=0){
            perror("errore nella creazione della pipe\n");
            exit(-2);
        }
    }

    //avvio degli N processi figli
    for(int i=0;i<N;i++){
        if((pid=fork())<0){
            perro("errore nella fork\n");
            exit(-2);
        }

        //avvio processo figlio, se figlio entra se padre continua il for
        if(pid==0){
            //chiusura delle pipe inutilizzate
            for(int j=0;j<N;j++){
                if(i!=j){
                    close(piped[j][1]);
                    close(pipedres[j][0]);
                }
                close(piped[j][0]);
                close(pipedres[j][1]);
            }


        }
    }

    //padre
    //chiusura delle pipe del padre
    for(int j=0;j<N;j++){
        close(piped[j][1]);
        close(pipedres[j][0]);
    }

    int index=0,res;
    int* c=calloc(N,1);
    while(read(piped[index][0],c[index],1)>0){
        

        index++;
        if(index==N){
            int max=0;
            for(int i=0;i<N;i++){
                if(c[i]>)
            }
            index=0;
        }
    }



    return 0;
}