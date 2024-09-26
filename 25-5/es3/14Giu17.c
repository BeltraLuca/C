#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

typedef int pipe_t[2];

int main(int argc, char** argv){

    
    if(argc<3){
        perror("numero parametri insufficiente\n");
        exit(-1);
    }

    if(strlen(argv[argc-1])!=1){
        perror("l'ultimo parametro non è un carattere\n");
        exit(-2);
    }
    char cx=argv[argc-1][0],c,t=0;
    long int count=0,counter=0,ordin[argc-2];
    int fd[argc-2],index=0,pid[argc-2];
    pipe_t *pipedc=calloc(argc-2,sizeof(pipe_t));
    pipe_t *pipedcou=calloc(argc-2,sizeof(pipe_t));
    
    for(int i=0;i<argc-2;i++){
        if(pipe(pipedcou[i])<0){
            perror("errore nella creazione della pipe");
            exit(-5);
        }
        if(pipe(pipedc[i])<0){
            perror("errore nella creazione della pipe");
            exit(-5);
        }
    }

    for(int i=1;i<argc-1;i++){
        t++;
        ordin[i-1]=t;
        if((pid[i-1]=fork())<0){
            perror("Errore nella creazione della fork\n");
            exit(-5);
        }
        if(pid[i-1]==0){
            break;
        }
    }

    for(int i=1;i<argc-1;i++){
        if(pid[i-1]==0){
            if(ordin[i-1]==t){
                printf("sono il processo figlio %d\n",getpid());
                if(argv[i][0]!='/'){
                perror("il nome passato per file non è un nome assoluto\n");
                exit(-4);
                }
                if((fd[index++]=open(argv[i],O_RDWR))<0){
                    perror("il file non è accessibile\n");
                    exit(-3);
                }
                close(pipedc[i-1][1]);
                close(pipedcou[i-1][0]);
                
                while(read(fd[index-1],&c,1)==1){
                    
                    count++;
                    if(c==cx){
                        printf("%d ho trovato all'indice %ld\n",i,count);
                        if(write(pipedcou[i-1][1],&count,sizeof(long int))<=0){
                            perror("errore di stampa indice");
                        }
                        if(read(pipedc[i-1][0],&c,1)<0){
                            perror("errore nella lettura del carattere dal figlio");
                        }
                        printf("\nil figlio ha ricevuto il carattere %c dal padre\n",c);
                        if(c!='\n'){
                            counter++;
                            lseek(fd[index-1],-1,SEEK_CUR);
                            write(fd[index-1],&c,1);
                        }
                    }
                }

                close(pipedc[i-1][0]);
                close(pipedcou[i-1][1]);
                printf("fine processo\n");
                if(count>=255){
                    exit(254);
                }
                
                exit(counter);
            }
        }
    }

    printf("sono il processo padre %d\n",getpid());
    bool entra=false;
    
    for(int i=0;i<argc-2;i++){
        close(pipedc[i][0]);
        close(pipedcou[i][1]);
    }

    while(entra==false){
        entra=true;
        
        for(int i=0;i<argc-2;i++){
            printf("il padre legge il figlio %d\n",i);
            if(read(pipedcou[i][0],&count,sizeof(long int))>=0){
                printf("\nil figlio numero %d con pid %d ha riportato un'occorrenza del carattere %c all'indice %ld nel file %s\n",i,pid[i],cx,count,argv[1+i]);
                printf("inserire il carattere da sostituire (se non si vuole sostituire non scrivere nulla): \n");
                char elem[2];
                read(0,&elem[0],1);
                if(elem[0]!='\n'){
                    read(0,&elem[1],1);
                }
                c=elem[0];
                printf("il padre scrive al figlio %d\n",i);
                write(pipedc[i][1],&c,1);
                entra=false;
            }else{
                printf("errore nella read\n");
            }
        }
        
    }

    int status,pidw;
    
    
    while(!((pidw=wait(&status))<0)){
        status=WEXITSTATUS(status);
        printf("il processo figlio con pid %d ha fatto %d sostituzioni\n",pidw,status);
    }
    exit(0);
}