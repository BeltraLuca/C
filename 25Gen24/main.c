#include <stdio.h>		// Includo la libreria per la funzione printf e BUFSIZ
#include <stdlib.h>		// Includo la libreria per la funzione exit
#include <unistd.h>		// Includo la libreria per la funzione close, fork, famiglia exec, read, write, lseek, famiglia get, pipe,
#include <fcntl.h>		// Includo la libreria per la funzione open, creat e le relative macro
#include <sys/wait.h>	// Includo la libreria per la funzione wait
#include <ctype.h>

//definisco il tipo pipe_t
typedef int pipe_t[2];
int* finito,N;

int finitof(){
    for(int i=0;i<N;i++){
        if(!finito[i]){
            return 0;
        }
    }
    return 1;
}

int main(int argc, char** argv) {
    N=argc-2;
    int n=1,nr=0,nw=0,ritorno=0;
    int pidFiglio;
    int fdw = 0; // variabile che conterra' il file descriptor del file argv[n]
    int fdr = 0; //variabile che conterra' il file descriptor dei file dei figli
    pipe_t* piped = (pipe_t*)malloc(sizeof(pipe_t) * (N));
    finito=calloc(N,sizeof(int));
    char ch[2]; 

   
    if(!(argc>=4)){
        printf("Errore, nell'inserimento dei paramteri\n");
        exit(1);
    }

    //apro il file argv[n]
    
    if((fdw = open(argv[argc-1], O_WRONLY)) < 0){		/* ERRORE se non si riesce ad aprire il file */
        printf("Errore in apertura file %s dato che fd = %d\n", argv[n], fdw);
        exit(2);
    }
    printf("ho aperto il file %s\n", argv[argc-1]);

    //apro N pipes
    
    if (piped == NULL) {
        printf("Errore nell'allocazione della memoria\n");
        exit(3);
    }
    
    for(int i = 0; i < N; i++){
        if(pipe(piped[i])<0){
            printf("Errore nella creazione della pipe numero:%d\n",i);
            exit(4);
        }
    }

    for(int n = 1; n <= N; n++){
    
        
        if ((pidFiglio = fork()) < 0)	/* Il processo padre crea un figlio */
        {	/* In questo caso la fork e' fallita */
            printf("Errore durante la fork\n");
            exit(5);
        }
        
        if (pidFiglio == 0)
        {	/* processo figlio */
            //chiudo tutte le pipe in lettura e scrittura tranne la i-esima pipe, aperta in lettura
            for(int k = 0; k < N; k++){
                close(piped[k][0]);
                if (k != n) {
                    close(piped[k][1]);
                }
            }

            while((nr=read(fdr,ch,2))!= 0){
                if((nw=write(piped[n-1],&ch,nr))!=nr){
                    printf("errore nell'invio di dati al padre\n");
                    exit(6);
                }
                ritorno=ritorno+nr;
            }

            exit(ritorno);
        }
    }
    /* processo padre, attende i 2 caratteri da ciascun figlio e li scrive sul file aperto all'inizio */

    //chiudo tutte le pipe in scrittura per tenere aperto tutte le pipe in lettura
    for(int k = 0; k < N; k++){
            close(piped[k][1]);
            
    }

    lseek(fdw,0L,SEEK_END);
    while(!finitof()){
        for(n=0;n<N;n++){
            nr=read(piped[n],ch,2);
            finito[n]= (nr==0);
            if(!finito[n]==0){
                write(fdw,ch,nr);
            }
              
        }
        
    }

    int status;
    /* Il padre aspetta i figli */
    for (n=0; n < N; n++)
    {
            pidFiglio = wait(&status);
            if (pidFiglio < 0)
            {
                    printf("Errore in wait\n");
                    exit(7);
            }
            if ((status & 0xFF) != 0)
                    printf("Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
            else
            { 
            ritorno=(int)((status >> 8) & 0xFF);
                printf("Il figlio con pid=%d ha ritornato %d (se 255 problemi)\n", pidFiglio, ritorno);
            } 
    }
    exit(0);
    
}