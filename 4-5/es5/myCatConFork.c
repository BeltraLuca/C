#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char** argv){

    if(argc!=2){
        printf("ERRORE inserire almeno un parametro");
        exit(1);
    }


    int pid;

    if((pid=fork())<0){
        printf("ERRORE NON FROKIA");
        exit(3);
    }

    //codice figlio
    if(pid==0){
        close(0);
        if(open(argv[1],O_RDWR)==-1){
            printf("%s non è un file esistente",argv[1]);
            exit(2);
        }
        printf("questo è il processo figlio con pid %d\n", getpid());
        execlp("./mycat", "mycat", (char *)0);
        exit(0);

    }

    printf("questo è il processo padre con pid %d\n",getpid());
}