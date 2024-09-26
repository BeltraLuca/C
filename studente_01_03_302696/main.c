#include <stdio.h>		// Includo la libreria per la funzione printf e BUFSIZ
#include <stdlib.h>		// Includo la libreria per la funzione exit
#include <unistd.h>		// Includo la libreria per la funzione close, fork, famiglia exec, read, write, lseek, famiglia get, pipe,
#include <fcntl.h>		// Includo la libreria per la funzione open, creat e le relative macro
#include <sys/wait.h>	// Includo la libreria per la funzione wait
#include <string.h>

//definisco il tipo pipe_t
typedef int pipe_t[2];

typedef struct struttura{
    long c1;    //valore minimo delle linee calcolato dal processo
    int c2;         //contine l'indice d'ordine (y) del processo che ha il valore minimo
}str;


int main(int argc, char** argv) {
    if (argc < 2 + 1) /* controllo sul numero di parametri: devono essere in numero maggiore o uguale a 2*/
    {
        printf("Errore: numero di argomenti sbagliato dato che argc = %d\n", argc);
        exit(0);
    }

    int Y=argc-1;                                                      //numero processi figli che a sua volta sono anche il numero delle pipe
    int y;                                                             //indice processi figli
    char* line[250];                                                   //linee lette dai figli nel proprio file
    int cur_tot=0;                                                       //calcolo del numero di linee effettuato dai figli
    long m=__LONG_MAX__;                                                //valore minimo ricevuto dai figli   
    int pid;
    int fd;
    int w=0;
    int j;
    str st;
    st.c1=__LONG_MAX__;
    int nw;

    //apro Fasi pipes
    pipe_t* piped = (pipe_t*)malloc(sizeof(pipe_t) * (Y));
    if (piped == NULL) {
        printf("Errore nell'allocazione della memoria\n");
        exit(1);
    }
    for(int i = 0; i < Y; i++){
        if(pipe(piped[i])<0){
            printf("Errore nella creazione della pipe numero:%d\n",i);
            exit(2);
        }
    }

    /* creazione figli */
    for (y=0;y<Y;y++)
    {
        if ((pid=fork())<0)
        {
            printf("Errore creazione figlio\n");
            exit(4);
        }
        if (pid == 0)
        { /* codice figlio */
        printf("Sono il figlio %d e sono associato al file %s\n", getpid(), argv[y+1]);
        /* nel caso di errore in un figlio decidiamo di ritornare un valore via via crescente rispetto al massimo valore di q (che e' Q-1) */
        /* chiusura pipes inutilizzate */

        for (j=0;j<Y;j++){
            if (j!=y)
                close (piped[j][0]);
            if ( j != (y+1)%Y)
                close (piped[j][1]);
        }

            /* apertura file */
        if ((fd=open(argv[y+1],O_RDONLY))<0)
        {
            printf("Impossibile aprire il file %s\n", argv[y+1]);
            exit(3);
        }
        
        //carichiamo il vettore con il testo del file
        while(read(fd,&line[w],1) != 0){
                w++;
                if(line[w]=='\n'){
                    cur_tot++;
                }
        }

        nw=read(piped[(y+1)%Y][1],&st,sizeof(str));

        if (nw != sizeof(str))
        {
                printf("Figlio %d ha letto un numero di byte sbagliati %d\n", y, nw);
                exit(Y+2);
        }

        //se presente una lettera alla fine aggiungere 1 al numero di linee
        if(line[w]!='\n'){
            cur_tot++;
        }

        if(st.c1>cur_tot){
            st.c1=cur_tot;
            st.c2=y;
        }
        
        /* ora si deve mandare l'OK in avanti: nota che il valore della variabile ok non ha importanza */
        nw=write(piped[(y+1)%Y][1],&st,sizeof(str));

        if (nw != sizeof(str))
        {
                printf("Figlio %d ha scritto un numero di byte sbagliati %d\n", y, nw);
                exit(Y+2);
        }

        nw=read(piped[(y+1)%Y][1],&st,sizeof(str));

        if (nw != sizeof(str))
        {
                printf("Figlio %d ha letto un numero di byte sbagliati %d\n", y, nw);
                exit(Y+2);
        }

        printf("sono il figlio %d, con pid= %d, il figlio numero %d ha il numero minimo di righe con %d righe, il mio file è %s, e la linea selezionata è \n",y,getpid,st.c2,st.c1,argv[y+1]);
        

        exit(y);
        }       //fine figlio

    }

    
	for(y=1;y<Y;y++){
        close(piped[y][0]);
    }

    st.c1=__LONG_MAX__;
    st.c2=0;
    nw=write(piped[1][1],&st,sizeof(str));
    if (nw != sizeof(str))
    {
            printf("il padre non ha scritto");
            exit(Y+2);
    }

    nw=read(piped[0][0],&st,sizeof(str));
    if (nw != sizeof(str))
    {
            printf("il padre non ha letto");
            exit(Y+2);
    }

    for(y=1;y<Y;y++){
        nw=write(piped[y][1],&st,sizeof(str));
        if (nw != sizeof(str))
        {
                printf("il padre non ha scritto");
                exit(Y+2);
        }
    }

    for(int i=0;i<Y;i++){
        int pidFiglio;
        int status;	// La variabile usata per memorizzare quanto ritornato dalla primitiva wait
        int ritorno;	// La variabile usata per memorizzare il valore di ritorno del processo figlio
        if ((pidFiglio = wait(&status)) < 0) {
            printf("Non e' stato creato nessun processo figlio\n");
            exit(5);
        }
        
        if ((status & 0xFF) != 0)
        {
            printf("Il processo figlio con PID %d è stato terminato in modo anomalo\n", pidFiglio);
        } else {
            ritorno = (status >> 8) & 0xFF;
            
        }
        printf("ha appena finito il figlio %d, con pid %d",ritorno,pidFiglio);
    }
    




    exit(0);
}