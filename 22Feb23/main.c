#include <stdio.h>		// Includo la libreria per la funzione printf e BUFSIZ
#include <stdlib.h>		// Includo la libreria per la funzione exit
#include <unistd.h>		// Includo la libreria per la funzione close, fork, famiglia exec, read, write, lseek, famiglia get, pipe,
#include <fcntl.h>		// Includo la libreria per la funzione open, creat e le relative macro
#include <sys/wait.h>	// Includo la libreria per la funzione wait

typedef int pipe_t[2];

int main(int argc, char** argv) {
    int N=argc-1,fs;
    long somma;
    int pidFiglio;	// memorizza il valore di ritorno della funzione fork
    int res,num=0;
    char car;
    if (argc < 2 + 1) /* controllo sul numero di parametri: devono essere in numero maggiore o uguale a 2*/
    {
        printf("Errore: numero di argomenti sbagliato dato che argc = %d\n", argc);
        exit(-1);
    }  

    //apro N pipes
    pipe_t* piped = (pipe_t*)malloc(sizeof(pipe_t) * (N));
    if (piped == NULL) {
        printf("Errore nell'allocazione della memoria\n");
        exit(-2);
    }
    
    for(int n = 0; n < N; n++){
        if(pipe(piped[n])<0){
            printf("Errore nella creazione della pipe numero:%d\n",n);
            exit(-3);
        }
    }

    for(int n = 0; n < N; n++){
        
        
        if ((pidFiglio = fork()) < 0)	/* Il processo padre crea un figlio */
        {	/* In questo caso la fork e' fallita */
            printf("Errore durante la fork\n");
            exit(-4);
        }
        
        if (pidFiglio == 0)
        {	/* processo figlio */
            somma=0;
            num=0;
            if((fs=open(argv[n+1],O_RDONLY))<0){
                printf("Errore nell`apertura del file\n");
                exit(-5);
            }

            //chiudo tutte le pipe in lettura e scrittura tranne la i-esima pipe, aperta in scrittura
            for(int k = 0; k < N; k++){
                close(piped[k][0]);
                if (k != n) {
                    close(piped[k][1]);
                }
            }

            
            while((res=read(fs,&car,1))==1){
                if(car>='0' && car<='9'){
                    somma+=car-'0';
                    num++;
                }
            }

            if((res=write(piped[n][1],&somma,sizeof(long)))<0){
                printf("errore nella scrittura sulla pipe54\n");
                exit(-6);
            }
            
    
            exit(num);
        }
    }
        /* processo padre */
        for(int k = 0; k < N; k++){
            close(piped[k][1]);
        }

        for(int n = 0; n < N; n++){
            if((res=read(piped[n][0],&somma,sizeof(long)))<0){
                printf("errore nella lettura sulla pipe\n");
                exit(-6);
            }
            printf("il figlio numero %d, associato al file %s ha inviato una somma di %ld\n",n,argv[n+1],somma);
        }

        
        int status;	// La variabile usata per memorizzare quanto ritornato dalla primitiva wait
        int ritorno;	// La variabile usata per memorizzare il valore di ritorno del processo figlio
        while((pidFiglio = wait(&status))>=0)
        
        if ((status & 0xFF) != 0)
        {
            printf("Il processo figlio con PID %d è stato terminato in modo anomalo\n", pidFiglio);
        } else {
            ritorno = (status >> 8) & 0xFF;
            printf("il figlio con pid %d, ha trovato %d numeri interni al file\n",pidFiglio,ritorno);
        }

    
    
    exit(0);
}