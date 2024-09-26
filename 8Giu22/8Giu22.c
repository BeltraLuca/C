#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#define MSG_SIZE 6

typedef int pipe_t[2];

int main(int argc, char** argv){

    if(argc<4){
        perror("Errore nell'inserimento dei parametri\n");
        exit(-1);
    }

    int fd;
    int pid;
    int status;
    int pidn;
    int N=argc-1;                                   //numero file
    int n;                                          //contatore processi figli
    char *linea=calloc(MSG_SIZE,sizeof(char));      //stringa che contiene la linea letta
    char *buffer=calloc(MSG_SIZE,sizeof(char));     //buffer per la lettura del nome dal figlio p0
    pipe_t *piped=calloc(N-1,sizeof(pipe_t));       //pipe

    for(int i=0;i<N-1;i++){
        if(pipe(piped[i])<0){
            perror("Errore nella creazione della pipe\n");
            exit(-3);
        }
    }

    for(int n=0;n<N;n++){
        if((pid=fork())<0){
            perror("errore nella creazione della fork\n");
            exit(-2);
        }

        if(pid==0){
            if(n==0){       //codice processo 0
                
                for(int i=0;i<N-1;i++){
                    close(piped[i][0]);
                }

                if((fd=open(argv[n+1],O_RDONLY))<0){
                    perror("errore nell'apertura del file\n");
                    exit(-4);
                }

                while((read(fd,linea,MSG_SIZE))==MSG_SIZE){
                    for(int i=0;i<N-1;i++){
                        if((write(piped[i][1],linea,MSG_SIZE))!=MSG_SIZE){
                            perror("errore nella scrittura in pipe\n");
                        }
                    }
                }
                exit(n);

            }else{          //codice processi figli bastardi
                for(int i=0;i<N-1;i++){
                    if((i+1)==n){
                        close(piped[i][1]);
                        continue;
                    }
                    close(piped[i][0]);
                    close(piped[i][1]);
                }

                if((fd=open(argv[n+1],O_RDONLY))<0){
                    perror("errore nell'apertura del file\n");
                    exit(-4);
                }

                while((read(piped[n-1][0],buffer,MSG_SIZE))==MSG_SIZE){
                    while((read(fd,linea,MSG_SIZE))==MSG_SIZE){
                        buffer[MSG_SIZE-1]='\0';
                        linea[MSG_SIZE-1]='\0';
                        if((pidn=fork())<0){
                            perror("errore nella creazione del nipote\n");
                            exit(-5);
                        }

                        if(pidn==0){
                            close(1);
                            open("/dev/null",O_WRONLY);
                            execlp("diff","diff",buffer,linea,(char*)0);
                            perror("errore nell'exec\n");
                            exit(-6);
                        }

                        if(wait(&status)<0){
                            perror("errore nella wait\n");
                            exit(-7);
                        }

                        status=WEXITSTATUS(status);

                        if(status==0){
                            printf("P%d:il file %s e %s sono uguali\n",n,buffer,linea);
                        }else{
                            printf("P%d:il file %s e %s sono diversi\n",n,buffer,linea);
                        }


                    }
                    lseek(fd,0,SEEK_SET);
                }
                exit(n);

            }
        }
        
    }

    for(int i=0;i<N-1;i++){      
        close(piped[i][0]);
        close(piped[i][1]);
    }

    for(int i=0;i<N;i++){
        if((pid=wait(&status))<0){
            perror("errore nella wait\n");
            exit(-8);
        }
        status=WEXITSTATUS(status);
        printf("il figlio %d ha ritornato %d\n",pid,status);
    }
    return 0;

}