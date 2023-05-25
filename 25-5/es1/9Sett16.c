#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#define SIZE sizeof(occur)*26

typedef struct {
    long int v2;
    char v1;
}occur;
typedef int pipe_t[2];


void stampa(occur* o){
    for(int i=0;i<26;i++){
        printf("\nstruct %d\nlettera %c \noccorrenze %ld\n\n",i,o[i].v1,o[i].v2);
    }
}

void scambia(occur* o1, occur* o2){
    occur temp=*o1;
    *o1=*o2;
    *o2=temp;
}

void bubbleSort(occur *o, int dim)
{ 
    int i; bool ordinato = false;
    while (dim>1 && !ordinato)
    { 
        ordinato = true; /* hp: è ordinato */
        for (i=0; i<dim-1; i++)
            if (o[i].v2<o[i+1].v2)
            {
                scambia(&o[i],&o[i+1]);
                ordinato = false;
            }
            dim--;
        }
}

int main(int argc, char** argv){
    //verifico che venga passato solo un parametro
    if(argc!=2){
        perror("Numero parametri errato\n");
        exit(-1);
    }

    int F,pid;
    char elem;
    occur *occ=malloc(SIZE);
    pipe_t *piped=calloc(27,sizeof(pipe_t));

    
    for(int i=0;i<27;i++){
        if((pipe(piped[i]))<0){
            perror("errore nella creazione della pipe\n");
            exit(-3);
        }
    }
    write(piped[0][1],occ,SIZE);
    
    
    for(int i='a';i<='z';i++){
        if((pid=fork())<0){ perror("non è stato possibile creare il figlio\n");continue;}
        //codice figlio
        if(pid==0){
            close(piped[i-'a'][1]);
            
            read(piped[i-'a'][0],occ,SIZE);
            
            if((F=open(argv[1],O_RDONLY))<0){perror("il file non viene aperto\n"); exit(-2);}
            occ[i-'a'].v2=0;
            while(read(F,&elem,1)==1){
                occ[i-'a'].v1=i;  
                if(elem==i){                
                    occ[i-'a'].v2++;
                }
            }
            if(write(piped[(i+1)-'a'][1],occ,SIZE)<0){
                printf("non è stato scritto su pipe %d\n",(i+1)-'a');
            }
            
            exit(0);
        }


    }
    
    //printf("\nsono nel padre\n");
    int status;
    for(int i=0;i<26;i++){
        close(piped[i][0]);
        close(piped[i][1]);
    }
    if((read(piped[26][0],occ,SIZE))<0){
        perror("wait non funzionante");
        exit(-2);
    }


    //stampa(occ);
    bubbleSort(occ,26);
    stampa(occ);


}