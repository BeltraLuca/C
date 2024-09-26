#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>

typedef int pipe_t[2];
typedef struct {
        long int c1;         	// pid figlio 
        int c2;   	    // numero occorrenze 
                } s_occ;

int main (int argc, char **argv)
{
        char** linea;    //buffer 256
	int N;          // numero di file 
	int pid;        // pid per fork 
	pipe_t *pipes;  // array di pipe 
        pipe_t annuncio;
	int i,j;        // contatori
	int fd;         // file descriptor 
	int pidFiglio, status, ritorno; // per valore di ritorno figli 
	s_occ *cur;     // array di strutture usate dal figlio corrente 
	int nr;         // variabile per salvare valori di ritorno di read su pipe 
        int min;        //variabile mantenuta da ciascun figlio, mantiene il valore minimo mandato dal padre

// controllo sul numero di parametri almeno 2 file
if (argc < 3)
{
        printf("Errore numero di parametri: infatti argc e' %d\n", argc);
        exit(1);
}

N = argc-1;
// stampa di debugging
printf("DEBUG-Numero di processi da creare %d\n", N);

// allocazione pipe 
if ((pipes=(pipe_t *)malloc(N*sizeof(pipe_t))) == NULL)
{
        printf("Errore allocazione pipe\n");
        exit(4);
}

// creazione pipe 
for (i=0;i<N;i++)
        if(pipe(pipes[i])<0)
        {
                printf("Errore creazione pipe\n");
                exit(5);
        }

if(pipe(annuncio)<0)
        {
                printf("Errore creazione pipe\n");
                exit(5);
        }
linea=calloc(256,sizeof(char*));
for(i=0;i<256;i++){
        linea[i]=calloc(256,sizeof(char)); 
}
// creazione figli 
for (i=0;i<N;i++)
{
        if ((pid=fork())<0)
        {
                printf("Errore creazione figli\n");
                exit(6);
        }
        if (pid==0)
        {       // codice figlio 
                printf("Sono il figlio di indice %d e pid %d associato al file %s\n", i, getpid(), argv[i+1]);
                // nel caso di errore in un figlio decidiamo di ritornare un valore via via crescente rispetto al massimo valore di i (cioe' rispetto N-1) 
                // chiusura pipes inutilizzate 
                
                for (j=0;j<N;j++)
                {
                        if (j!=i)
                                close (pipes[j][1]);
                        if ((i == 0) || (j != i-1))
                                close (pipes[j][0]);
                }

                if(i!=0){
                        close(annuncio[0]);
                }
                close(annuncio[1]);

                // allocazione dell'array di strutture specifico di questo figlio 
                // creiamo un array di dimensione i+1 anche se leggeremo i strutture, dato che poi ci servira' riempire la i+1-esima struttura! 
                if ((cur=(s_occ *)malloc((i+1)*sizeof(s_occ))) == NULL)
                {
                        printf("Errore allocazione cur\n");
                        exit(N);
                }
                //inizializziamo l'ultima struttura (quella di posto i+1 che pero' ha chiaramente indice i) che e' quella specifica del figlio corrente (nel caso del primo figlio sara' l'unica struttura 
                cur[i].c1 = getpid();
                cur[i].c2= 0;

                //apertura file
                if ((fd=open(argv[i+1],O_RDONLY))<0)
                {
                        printf("Impossibile aprire il file %s\n", argv[i+1]);
                        exit(N+1);
                }
                j=0;
                while(read(fd,&linea[cur[i].c2][j],sizeof(char))>0)
                {     
                        if(linea[cur[i].c2][j]=='\n'){
                                (cur[i].c2)++;
                                j=0;
                        }else{
                                j++;
                        }
                    
                }
                if (i!=0)
                // lettura da pipe dell'array di strutture per tutti i figli a parte il primo 
                {
                        nr=read(pipes[i-1][0],cur,i*sizeof(s_occ));
                        if (nr != i*sizeof(s_occ))
                        {
                                printf("Figlio %d ha letto un numero di strutture sbagliate %d\n", i, nr);
                                exit(N+2);
                        }
                }
                write(pipes[i][1],cur,(i+1)*sizeof(s_occ));
                int s;
                if(i==0){
                        nr=read(annuncio[0],&s,sizeof(int));
                }else{
                        nr=read(pipes[i-1][0],&s,sizeof(int));  
                }

                
                printf("la linea %d del figlio %d e' %s\n",(s+1),getpid(),linea[s]);
                write(pipes[i][1],&s,sizeof(int));
                exit(i); // ogni figlio deve ritornare al padre il proprio indice 
        }
} 

// codice del padre 
// chiusura pipe: tutte meno l'ultima in lettura 
for(i=0;i<N;i++)
{
        close (pipes[i][1]);
        if (i != N-1) close (pipes[i][0]);
}
// allocazione dell'array di strutture specifico per il padre 
// creiamo un array di dimensione N quanto il numero di figli! 
if ((cur=(s_occ *)malloc(N*sizeof(s_occ))) == NULL)
{
        printf("Errore allocazione cur nel padre\n");
        exit(7);
}

// il padre deve leggere l'array di strutture che gli arriva dall'ultimo figlio 
nr=read(pipes[N-1][0],cur,N*sizeof(s_occ));
if (nr != N*sizeof(s_occ))
{
        printf("Padre ha letto un numero di strutture sbagliate %d\n", nr);
        exit(8);
}
nr=nr/sizeof(s_occ);
printf("Padre ha letto un numero di strutture %d\n", nr);
// il padre deve stampare i campi delle strutture ricevute
for(i=0;i<N;i++){
    if(min>cur[i].c2){
        min=cur[i].c2;
    }
    printf("Il figlio di indice %d e pid %ld ha trovato %d linee nel file %s\n", i, cur[i].c1, cur[i].c2+1, argv[i+1]);
}

// il padre deve stampare i campi delle strutture ricevute
for(i=0;i<N;i++){
    if(min==cur[i].c2){
        printf("\nil figlio %d ha il file %s con il numero minimo di linee %d \n",getpid(),argv[i+1],cur[i].c2+1);
    }
    
}

write(annuncio[1],&min,sizeof(int));
// Il padre aspetta i figli 
for (i=0; i < N; i++)
{
        pidFiglio = wait(&status);
        if (pidFiglio < 0)
        {
                printf("Errore in wait\n");
                exit(9);
        }

        if ((status & 0xFF) != 0)
                printf("Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
        else
        {
                ritorno=(int)((status >> 8) & 0xFF);
                printf("Il figlio con pid=%d ha ritornato %d (se > di %d problemi)\n", pidFiglio, ritorno, N-1);
        }
}
exit(0);
}