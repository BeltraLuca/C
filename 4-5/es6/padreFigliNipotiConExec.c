#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char** argv){
    if(argc <4){
        printf("ERRORE inserire almeno 3 parametri");
        exit(1);
    }

    int file;
    for(int i=1;i<argc;i++){
        file=open(argv[i],O_RDWR);
        if(file==-1){
            printf("ERRORE IL NOME PASSATO COME PARAMETRO %d NON VIENE APERTO",i);
            exit(2);
        }
        close(file);
    }

    int pid=1;

    for(int i=1;i<argc;i++){
        if((pid=fork())<0){
            printf("ERRORE la fork non forka");
            exit(3);
        }
        //codice figlio
        if(pid==0){
            printf("processo figlio con pid %d", getpid());
        }
    
    }

    //codice padre
    printf("processo padre con pid %d",getpid());
    int pidf;
    int status;
    if((pidf=wait(&status))<0){
        printf("errore nel wait");
        exit(4);
    }

    if((status&0xff)!=0){
        printf("errore nel wait");
        exit(4);
    }else{
        status=(int) (status >> 8) & 0xff;za                    
    }

}