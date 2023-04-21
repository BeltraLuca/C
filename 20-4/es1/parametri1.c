#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char** argv){
    if(argc<2){
        printf("inserire almeno un parametro");
        exit(1);
    }

    printf("\nil numero dei parametri= %d\nil nome dell'eseguibile= %s\n", argc-1, argv[0]);
    for(int i=1;i<argc;i++){
        printf("parametro %d è %s\n",i,argv[i]);
    }

}