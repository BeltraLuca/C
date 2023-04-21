#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int contaOccorrenze(int fo,char* c){
    int res;
    char buff;
    int count=0;
    
    while((res=read(fo,&buff,1))>0){

        if(buff==*c){
            count++;
        }
    }
    return count;

}

int main(int argc, char** argv){
    if(argc!=3){
        printf("inserire esattamente 3 parametri\n");
        exit(1);
    }

    int fo;
    if((fo=open(argv[1],O_RDONLY))==-1){
        printf("Il file non esiste o non hai i permessi\n");
        exit(2);
    }

    if(strlen(argv[2])<1){
        printf("il secondo parametro non è un carattere");
        exit(3);
    }


    printf("\nil numero dei parametri= %d\nil nome dell'eseguibile= %s\n", argc-1, argv[0]);
    printf("il parametro 1 è un file esistente= %s\n",argv[1]);
    printf("il parametro 2 è un carattere= %s\n",argv[2]);
    int res=contaOccorrenze(fo,argv[2]);
    printf("le occorrenze presenti sono= %d\n",res);

}