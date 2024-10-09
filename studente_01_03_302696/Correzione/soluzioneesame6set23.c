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
        long int c1;         	// numero di occorrenze 
        int c2;   	        // numero d'ordine 
                } s_occ;

int main (int argc, char **argv)
{
        long int act=0;                      //numero di caratteri contati
        char car;                       //carattere
        char linea[250];                    
	int N;                          // numero di file  
	pipe_t *pipes;                  // array di pipe 
        pipe_t *comunicazione;          //array di pipe per comunicazione padre figlio
	int i,j,o;                        // contatori
	int fd;                         // file descriptor 
	int pidFiglio, status, ritorno; // per valore di ritorno figli 
	s_occ cur;                      // struttura dati usata dal figlio corrente 
	int nr;                         // variabile per salvare valori di ritorno di read su pipe 
      
// controllo sul numero di parametri almeno 2 file
if (argc < 3)
{
        printf("Errore numero di parametri: infatti argc e' %d\n", argc);
        exit(1);
}

N = argc-1;
int pid[N];                     // pid per fork
// stampa di debugging
printf("DEBUG-Numero di processi da creare %d\n", N);

// allocazione pipe 
if ((pipes=(pipe_t *)malloc(N*sizeof(pipe_t))) == NULL)
{
        printf("Errore allocazione pipe\n");
        exit(4);
}


// allocazione pipe 
if ((comunicazione=(pipe_t *)malloc(N*sizeof(pipe_t))) == NULL)
{
        printf("Errore allocazione pipe\n");
        exit(4);
}

// creazione pipe 
for(i=0;i<N;i++){
        if(pipe(pipes[i])<0)
        {
                printf("Errore creazione pipe\n");
                exit(5);
        }

        if(pipe(comunicazione[i])<0)
        {
                printf("Errore creazione pipe\n");
                exit(5);
        }
}

cur.c1=__LONG_MAX__;
// creazione figli 
for (i=0;i<N;i++)
{
        if ((pid[i]=fork())<0)
        {
                printf("Errore creazione figli\n");
                exit(6);
        }
        if (pid[i]==0)
        {       // codice figlio 
                printf("Sono il figlio di indice %d e pid %d associato al file %s\n", i, getpid(), argv[i+1]);
                // nel caso di errore in un figlio decidiamo di ritornare un valore via via crescente rispetto al massimo valore di i (cioe' rispetto N-1) 
                // chiusura pipes inutilizzate 
                
                for (j=0;j<N;j++)
                {
                        if (j!=i){
                                close (pipes[j][1]);
                                close(comunicazione[j][0]);
                        }
                        if ((i == 0) || (j != i-1)){
                                close (pipes[j][0]);
                        }
                        
                        close(comunicazione[j][1]);

                }

                

                //apertura file
                if ((fd=open(argv[i+1],O_RDONLY))<0)
                {
                        printf("Impossibile aprire il file %s\n", argv[i+1]);
                        exit(N+1);
                }
                j=0;
                
                while(read(fd,&car,sizeof(char))>0)
                {     
                        if(car=='\n'){
                                act++;
                                j=0;
                        }else{
                                j++;
                        }
                    
                }
                if (i!=0)
                // lettura da pipe dell'array di strutture per tutti i figli a parte il primo 
                {
                        nr=read(pipes[i-1][0],&cur,sizeof(s_occ));
                        if (nr != i*sizeof(s_occ))
                        {
                                printf("Figlio %d ha letto un numero di strutture sbagliate %d\n", i, nr);
                                exit(N+2);
                        }
                }
                if(act<cur.c1){
                        cur.c1=act;
                        cur.c2=i;
                }
                
               
                write(pipes[i][1],&cur,sizeof(s_occ));
                long int s;
                nr=read(comunicazione[i][0],&s,sizeof(long int));
                
                lseek(fd,0,SEEK_SET);
                j=0;
                o=0;
                while(read(fd,&car,sizeof(char))>0)
                {     
                        
                        if(j==s){
                                linea[o]=car;
                                o++;
                                if(car=='\n'){
                                        linea[o]=(char)0;
                                        break;
                                }
                        }
                        if(car=='\n'){
                                j++;
                        }
                    
                }
                printf("sono il figlio %d con pid %d\n%ld° linea nel file %s -->  %s\n\n",i,getpid(),(s+1),argv[i+1],linea);
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


// il padre deve leggere l'array di strutture che gli arriva dall'ultimo figlio 
nr=read(pipes[N-1][0],&cur,sizeof(s_occ));
if (nr != sizeof(s_occ))
{
        printf("Padre ha letto un numero di strutture sbagliate %d\n", nr);
        exit(8);
}
// il padre deve stampare i campi delle strutture ricevute
printf("al padre è arrivata la seguente struttura:\ncur.c1= %ld\ncur.c2= %d\n\n",cur.c1,cur.c2);
printf("il figlio con numero di occorenze minore è il nr= %d con pid= %d alla quale è associato il file %s in cui a trovato %ld linee\n",cur.c2,pid[cur.c2],argv[cur.c2+1],cur.c1);

for(int i=0;i<N;i++){
        write(comunicazione[i][1],&cur.c1,sizeof(long int));
}

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
                printf("Il figlio con pid=%d ha ritornato %d \n", pidFiglio, ritorno);
        }
}
exit(0);
}
