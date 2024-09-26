#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <ctype.h>

typedef int pipe_t[2];

int main(int argc, char** argv){
    int N=argc-1,pid,pidn,fd,fdn;

    if(argc<3){
        perror("parametri insufficienti, inserire almeno due parametri\n");
        exit(-1);
    }

    pipe_t* piped=calloc(N,sizeof(pipe_t));
    pipe_t* pipedn=calloc(N,sizeof(pipe_t));

    for(int j=0;j<N;j++){
            if(pipe(piped[j])<0){
                perror("errore nella creazione di pipe\n");
                exit(-2);
            }

            if(pipe(pipedn[j])<0){
                perror("errore nella creazione di pipe\n");
                exit(-2);
            }
    }

    //ciclo i per la scelta del file e pipe 
    for(int i=0;i<N;i++){
        
        //creazione file descriptor per il figlio
        if((fd=open(argv[i+1],O_RDWR))<0){
            printf("%d\n",i);
            perror("Errore nell'apertura del file\n");
            continue;
        }
        //creazione file descriptor per il nipote
        if((fdn=open(argv[i+1],O_RDWR))<0){
            printf("%s\n",argv[i+1]);
            perror("Errore nell'apertura del file\n");
            continue;
        }

        //verifica dell'avenuta creazione della fork
        if((pid=fork())<0){
            perror("errore nella creazione del processo figlio\n");
            exit(-3);
        }

    //inizio processo figlio
        if(pid==0){
            close(piped[i][0]);
            //verifica dell'avenuta creazione della fork
            if((pidn=fork())<0){
            perror("errore nella creazione del processo figlio\n");
            exit(-3);
            }
            //inizio processo nipote
            if(pidn==0){
                close(pipedn[i][0]);
                long int count=0;
                char c;
                while(read(fdn,&c,1)==1){
                    if(islower(c)){
                        count++;
                        c=toupper(c);
                        lseek(fdn,-1,SEEK_CUR);
                        if(write(fdn,&c,1)!=1){
                            perror("non ho trasformato\n");
                        }
                    }
                }
                write(pipedn[i][1],&count, sizeof(long int));
                exit((int) count/256);
            }

            long int count=0;
            char c;
            while(read(fd,&c,1)==1){
                if(isdigit(c)){
                    count++;
                    lseek(fd,-1,SEEK_CUR);
                    write(fd,(char*)" ",1);
                }
            }

            write(piped[i][1],&count, sizeof(long int));

            int status;
            if((wait(&status))<0){
                perror("Errore nella wait\n");
                exit(-4);
            }

            status=WEXITSTATUS(status);
            printf("finito il nipote con pid %d e ha ritornato %d\n",pidn,status);
            exit((int) count/256);
        }
        int status=0;
        if((pid=wait(&status))<0){
            perror("Errore nella wait\n");
            exit(-4);
        }

        status=WEXITSTATUS(status);
        printf("finito il figlio con pid %d e ha ritornato %d\n",pid,status);

    }
    
    long int res, resn;
    for(int i=0;i<N;i++){
        close(piped[i][1]);
        close(pipedn[i][1]);
        read(piped[i][0],&res,sizeof(long int));
        read(pipedn[i][0],&resn,sizeof(long int));

        printf("il file di nome %s ha avuto %ld trasformazioni di tipo numerico dal figlio e %ld trasformazioni di tipo alfabetico dal nipote\n",argv[i+1],res,resn);
    }

    
}