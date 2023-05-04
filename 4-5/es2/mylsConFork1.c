#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char** argv){

    int pid;

    if((pid=fork())<0){
        printf("ERRORE IL FROKIO NON FUNZIONA");
        exit(1);
    }

    //codcie figlio
    if(pid==0){
        printf("sono il figlio con pid %d\n", getpid());
        execlp("ls","ls", "-l", argv[1], (char*)0);

    }
    printf("sono il padre con pid %d\n", getpid());
    exit(2);

    
}