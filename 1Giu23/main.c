#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

typedef int pipe_t[2];

typedef struct{
    int c1;             //pid nipote
    char c2[250];       //ultima linea
    int c3;             //lunghezza linea
} struct_t;


//ritorna la lunghezza della stringa senza terminatore
int lunghezza(char* str){
    int res=0;
    while(str[res]!='\n'){
        res++;
    }
    return res;
}

int main(int argc, char** argv){

    if(argc<2){
        perror("errore nell'inserimento dei parametri\n");
        exit(-1);
    }

    int K=argc-1;                                   //numero file
    int pid;                                        //pid figli
    int pidn;                                       //pid nipoti
    int status;                                     //status per acquisire il valore di ritorno dei figli
    pipe_t *pf=calloc(K,sizeof(pipe_t));            //pipe padre figli
    char buffer[250];                               //buffer per acquisire le linee del rev
    struct_t *st=calloc(1,sizeof(struct_t));        //structure


    if( pf==NULL || st==NULL){                      //verifico siano stati allocati gli elementi
        perror("errore nella calloc");
        exit(-3);
    }


    for(int j=0;j<K;j++){                           //apro le K pipe padre figli
        if(pipe(pf[j])<0){
            perror("errore nella pipe\n");
            exit(-2);
        }
    }

    for(int k=0;k<K;k++){                           //creo K figli

        if((pid=fork())<0){                         //creo K figli pt2
            perror("errore nella creazione della fork()\n");
            exit(-4);
        }

        if(pid==0){                                 //codice figlio
            for(int i=0;i<K;i++){
                if(i!=k){
                    close(pf[i][1]);                //chiudo tutte le pipe in scrittura tranne quelle del figlio, il figlio deve scrivere solo nella propria pipe
                }
                close(pf[i][0]);                    //chiudo tutte le pipe in lettura, il figlio non deve leggere nulla nella propria pipe
            }

            pipe_t pipedn;                          //dichiaro la pipe di comunicazione del nipote nel figlio perchè la consegna richiedeva che il padre non potesse comunicare col nipote

            if(pipe(pipedn)<0){                     //creo la pipe del nipote
                perror("errore nella pipe\n");
                exit(-2);
            }

            if((pidn=fork())<0){                    //creo il nipote
                perror("errore nella fork\n");
                exit(-4);
            }

            if(pidn==0){                            //codice nipote
                close(1);                           //chiudo lo std output
                close(pf[k][1]);                    //chiudo la scrittura della pipe padre figlio, del padre di questo processo (nipote)
                close(pipedn[0]);                   //chiudo la pipe del nipote in lettura, in quanto il nipote non deve leggere nulla
                dup(pipedn[1]);                     //dupplico la scrittura del pipe del nipote, così da posizionare come nuovo std output la pipe del nipote
                execlp("rev","rev",argv[k+1],(char*)0);    //eseguo l'exec che mi fa il reverse del file
                exit(-1);                                   //nel caso l'exec non vada uscire con errore -1
            }
            close(pipedn[1]);                               //il figlio chiude la pipe del nipote in scrittura, in quanto non deve scrivere nulla
            int indice=0;                                   //indice del carattere del buffer che viene letta dalla pipe nipote
            char c;                                         //lettura del carattere successivo, per verificare che sia effettivamente l'ultima linea
            while(read(pipedn[0],&buffer[indice],1)>=0){    //lettura carattere per carattere della pipe
                if(buffer[indice]=='\n'){                   //se presente il terminatore di linea, verifica la presenza di un carattere successivo
                    if((indice=read(pipedn[0],&c,1))>=0){
                        if(indice==1){
                            memset(buffer,' ',250);         //se presente il carattere successivo libera il buffer
                            indice=1;                       //imposta l'indice a 1
                            buffer[indice-1]=c;             //modifica il primo carattere del buffer con il carattere c di verifica nuova linea
                        }else{
                            break;                          //se non è presente nessuna linea dopo il terminatore di linea, termina il while
                        }
                    }
                }else{
                    indice++;                               //se il carattere estratto non è un terminatore di linea, aumenta l'indice così da continuare la stringa
                }
            }
            
            if((st->c1=wait(&status))<0){                   //wait del figlio, la wait ritorna il pid del nipote, che noi metteremo all'interno della struct, non ci interessa di status
                perror("errore nella wait\n");
                exit(-6);
            }
            memcpy(st->c2,buffer,250);                      //copiamo il buffer nella struct
            st->c3=lunghezza(st->c2);                       //e anche la lunghezza della stringa senza terminatore

            if(write(pf[k][1],st,sizeof(struct_t))<0){      //mandiamo la struct al padre, e al contempo verifichiamo che la write sia avvenuta con successo
                perror("errore nella scrittura\n");   
                exit(-9);
            }
            exit(lunghezza(st->c2)+1);                     //terminiamo il processo ritornando la lunghezza della stringa contando il terminatore
        }
    }

    for(int i=0;i<K;i++){
        close(pf[i][1]);                                    //siamo nel padre, e chiudiamo in scrittura tutte le pipe padre figlio, in quanto dobbiamo solo leggere
    }

    for(int i=0;i<K;i++){
        if(read(pf[i][0],st,sizeof(struct_t))<0){           //leggiamo la pipe per estrarre la struct 
                perror("errore nella lettura\n");
                exit(-9);
        }
        printf("\n\n%d figlio\npid nipote= %d\nlunghezza senza terminatore=%d\nultima linea= %s\n",i,st->c1,st->c3,st->c2);     //stampiamo la struct estratta in ordine 
    }

    
    for(int i=0;i<K;i++){                                                                                           //verifichiamo il valore di ritorno di ciascun figlio
        if((pid=wait(&status))<0){
                perror("errore nella wait\n");
                exit(-6);
        }

        if ((status & 0xFF) != 0)                                                                                   //verifichiamo che i bit bassi siano uguali a 0
        {
            printf("Il processo figlio è stato terminato in modo anomalo\n");
        } else {
            status = (status >> 8) & 0xFF;                                                                          //successivamente shiftiamo di 8 e estraiamo i soli 8 bit bassi, cioè il valore di ritorno
            printf("Il figlio con pid %d ha ritornato %d lunghezza con terminatore \n", pid, status);               //stampa pid e status
        }
    }
    exit(0);
}