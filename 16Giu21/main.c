#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#define PERM 0666
#define linelen 255
#define numline 4

typedef int pipe_t[2];
typedef char line[linelen];


int main( int argc, char** argv){

    if(argc<3){
        perror("errore numero parametri insuffficienti\n");
        exit(1);
    }

    int fcreato;
    int fd;
    int indice=0;
    int counte;
    int N=argc-1;
    line linea;
    line arrline[numline*N];

    int pid;
    pipe_t *piped=calloc(N,sizeof(pipe_t));

    printf("pipe create\n");
    for(int n=0;n<N;n++){
        if((pipe(piped[n]))<0){
            perror("Errore nella creazione della pipe");
            exit(5);
        }
    }
    
    printf("file creato\n");
    if((fcreato=creat("Beltrami",PERM))<0){
        perror("errore nella creazione del file\n");
        exit(2);
    }

    write(piped[0][1],&arrline,sizeof(arrline));
    printf("creazione figli\n");
    for(int n=0;n<N;n++){
        if((pid=fork())<0){
            perror("Errore nella creazione della fork\n");
            exit(3);
        }
        if(pid==0){
            printf("figlio %d creato\n",getpid());
            /*for(int i=0;i<N;i++){
                if(i!=n){
                    if(n!=(N-1)){
                        close(piped[i][1]);
                        printf("chiudo pipe[%d][1]",i);
                    }
                }
                if(i!=(n+1)){
                    if(n==(N-1)){
                        close(piped[0][0]);
                        printf("chiudo pipe[0][0]");
                    }else{
                        close(piped[i][0]);
                        printf("chiudo pipe[%d][0]",i);
                    }
                }
            }*/

            /* chiusura pipes inutilizzate */
            for (int j=0;j<N;j++)
            {	/* si veda commento nella definizione dell'array pipes per comprendere le chiusure */
                if (j!=n)
                    close (piped[j][0]);
                if (j != (n+1)%N)
                    close (piped[j][1]);
            }	
	
                               
            if((fd=open(argv[n+1],O_RDONLY))<0){
                perror("errore nell'apertura del file\n");
                exit(4);
            }
            printf("leggo da figlio precedente");
            while(read(piped[n][0],&arrline,sizeof(arrline))){
                counte=n*numline;
                indice=0;
                while(read(fd,&linea[indice],1)>0){
                    if(linea[indice]=='\n'){
                        if(write(piped[n+1][1],&linea,indice+1)<=0){
                            perror("errore nella write\n");
                        }
                        memcpy(arrline[counte],linea,linelen);
                        if(n==(N-1)){
                            write(piped[0][1],&arrline,sizeof(arrline));
                        }else{
                            write(piped[n+1][1],&arrline,sizeof(arrline));
                        }
                        break;     
                    }else{
                        indice++;
                    }
                }
            }
            printf("fine figlio %d",getpid());
            
        }
    }

    for(int n=0;n<N;n++){
        close(piped[n][0]);
        close(piped[n][1]);
    }

    
    int status;
    for(int n=0;n<N;n++){
        if((pid=wait(&status))<0){
            perror("errore nella wait\n");
            exit(9);
        }

        if(status & 0xFF != 0){
            perror("errore nel bit basso di status");
            continue;
        }

        if((status= status >> 8 & 0xFF) != 0){
            printf("il figlio %d ha ritornato %d (se 255= errore)",pid, status);
        }
    }
    return 0;
}