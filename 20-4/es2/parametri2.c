#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char** argv){
    if(argc!=4){
        printf("\ni parametri non sono 3");
        exit(1);
    }

    int fo;
    if(fo=open(argv[1],O_RDONLY)==-1){
        printf("il primo parametro non è un file o il file non è leggibile\n");
        exit(4);
    }
    if(atoi(argv[2])<=0){
        printf("il parametro 2 passato non un numero\n");
        exit(2);
    }
    

    if(strlen(argv[3])!=1){
        printf("il parametro 3 non è un unico carattere\n");
        exit(3);
    }

    printf("\nil numero dei parametri= %d\nil nome dell'eseguibile= %s\n", argc-1, argv[0]);
    printf("il parametro 1 è un file esistente= %s\n",argv[1]);
    printf("il parametro 2 è un numero= %s\n",argv[2]);
    printf("il parametro 3 è un carattere= %s\n",argv[3]);

    exit(5);
}