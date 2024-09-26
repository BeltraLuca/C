#include <stdio.h>		// Includo la libreria per la funzione printf e BUFSIZ
#include <stdlib.h>		// Includo la libreria per la funzione exit
#include <unistd.h>		// Includo la libreria per la funzione close, fork, famiglia exec, read, write, lseek, famiglia get, pipe,
#include <fcntl.h>		// Includo la libreria per la funzione open, creat e le relative macro
#include <sys/wait.h>	// Includo la libreria per la funzione wait
#include <string.h>
#define ExitValue 255
//definisco il tipo pipe_t
typedef int pipe_t[2];

int main(int argc, char** argv) {
    
    if (argc < 2 + 1) /* controllo sul numero di parametri: devono essere in numero maggiore o uguale a 2*/
    {
        printf("Errore: numero di argomenti sbagliato dato che argc = %d\n", argc);
        exit(ExitValue);
    }
    int N=argc-1;
    int fd = 0; // variabile che conterra il file descriptor del file file */
    char* file="/tmp/LucaBeltrami";
    pipe_t piped;
    int pidFiglio;	// memorizza il valore di ritorno della funzione fork
    char *buffer=calloc(BUFSIZ,sizeof(char));    // buffer che verra' letto da file
    int nread;              // variabile che memorizzera' il ritorno della read
    pipe_t* pipedN = (pipe_t*)malloc(sizeof(pipe_t) * (N));
    char c;	// carattere usato per leggere il contenuto del file
    int indice=0;
    int status;	// La variabile usata per memorizzare quanto ritornato dalla primitiva wait
    int ritorno;	// La variabile usata per memorizzare il valore di ritorno del processo figlio
    int z=0;
    int res;
    int len;
    int max;
    

    for(int i=1;i<=N;i++){
        if(argv[i][0]!='/'){
            printf("Errore, il %d parametro non è un nome assoluto\n",i); 
            exit(ExitValue);
        }
    }

    //creo il file file
    
    if((fd = creat(file, 0666)) < 0){		/* ERRORE se non si riesce a creare il file */
        printf("Errore in creazione del file %s dato che fd = %d\n", file, fd);
        exit(ExitValue);
    }
    
    
    if (pipe(piped) < 0 )
    {	printf("Errore creazione pipe\n");
        exit(ExitValue);
    }
    
    if ((pidFiglio = fork()) < 0)	/* Il processo padre crea un figlio speciale*/
    {	/* In questo caso la fork e' fallita */
        printf("Errore durante la fork\n");
        exit(ExitValue);
    }
    
    if (pidFiglio == 0)
    {	/* processo figlio */
        // chiudo lo stdout
        close(1);
        // duplico la pipe lato  scrittura
        dup(piped[1]);
        // chiudo il lato della pipe duplicato
        close(piped[1]);
        close(piped[0]);                    //chiusura in lettura
        execlp("wc","wc",argv[1],(char*)0); // in caso di lista, ricordati di 0 terminare la lista dei parametri! (char*)0
        /* si esegue l'istruzione seguente SOLO in caso di fallimento */
        perror("errore esecuzione comando"); // #include <errno.h>
        exit(ExitValue);
        exit(0);
    }
    /* processo padre */

    close(piped[1]);
    while ((nread = read(piped[0], buffer, BUFSIZ)) > 0 )	/* ciclo di lettura fino a che nread e' maggiore di 0 */
    {
        nread=atoi(buffer);
    }
    close(piped[0]);

    //apro Fasi pipes
    if (pipedN == NULL) {
        printf("Errore nell'allocazione della memoria\n");
        exit(ExitValue);
    }
    
    for(int i = 0; i < N; i++){
        if(pipe(pipedN[i])<0){
            printf("Errore nella creazione della pipe numero:%d\n",i);
            exit(ExitValue);
        }
    }

    for(int i = 0; i < N; i++){
        
        if ((pidFiglio = fork()) < 0)	/* Il processo padre crea un figlio */
        {	/* In questo caso la fork e' fallita */
            printf("Errore durante la fork\n");
            exit(ExitValue);
        }
        
        if (pidFiglio == 0)
        {	/* processo figlio */
            //chiudo tutte le pipe in lettura e scrittura tranne la i-esima pipe, aperta in scrittura
            for(int k = 0; k < N; k++){
                close(pipedN[k][0]);
                if (k != i) {
                    close(pipedN[k][1]);
                }
            }
            
            
            memset(buffer,' ',BUFSIZ);
            //apro il file file
            if((fd = open(argv[i+1], O_RDONLY)) < 0){		/* ERRORE se non si riesce ad aprire il file */
                printf("Errore in apertura file %s dato che fd = %d\n", file, fd);
                exit(ExitValue);
            }
            
            while (read(fd, &c, sizeof(char)) > 0)	/* ciclo di lettura fino a che riesco a leggere un carattere da file */
            {
                if((buffer[indice]=c)=='\n'){
                    len=indice+1;
                    write(pipedN[i][1],&len,sizeof(int));
                    write(pipedN[i][1],buffer,len);
                    max=indice;
                    indice=0;
                    memset(buffer,' ',BUFSIZ);
                }else{
                    buffer[indice]=c;
                    indice++;
                }
            }
            exit(max);

        }
    }

    //chiudo tutte le pipe in scrittura
    for(int k = 0; k < N; k++){
        close(pipedN[k][1]);
    }

    close(fd);
    //apro il file in append
    if((fd = open(file, O_WRONLY)) < 0){		/* ERRORE se non si riesce ad aprire il file */
        printf("Errore in apertura file %s dato che fd = %d\n", file, fd);
        exit(ExitValue);
    }

    while((nread = read(pipedN[z][0], &res, sizeof(int))) > 0 )	/* ciclo di lettura fino a che nread e' maggiore di 0 */
    {
        free(buffer);
        buffer=calloc(res,sizeof(char));
        memset(buffer,' ',res);
        if(read(pipedN[z][0],buffer,res)==res){
            
            printf("il figlio %d ha inviato %s\n",z,buffer);
            write(fd,buffer,res);
            if(z+1==N){
                z=0;
            }else{
                z++;
            }
        }
    }

    //chiudo tutte le pipe in lettura
    for(int k = 0; k < N; k++){
        close(pipedN[k][0]);
    }
    
    for(int k = 0; k < N; k++){
        
        if ((pidFiglio = wait(&status)) < 0) {
            printf("Non e' stato creato nessun processo figlio\n");
            exit(ExitValue);
        }
        
        if ((status & 0xFF) != 0)
        {
            printf("Il processo figlio con PID %d è stato terminato in modo anomalo\n", pidFiglio);
        } else {
            ritorno = (status >> 8) & 0xFF;
            printf("Il figlio di PID %d ha ritornato %d\n",pidFiglio,ritorno);
            
        }
    }

    exit(0);
}