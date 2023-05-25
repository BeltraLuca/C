#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{       int nread;                      /* valore ritorno read */
	char buffer[BUFSIZ];            /* usato per i caratteri */
	int fd = 0;			/* usato per la open; N.B. se non viene passato alcun parametro allora rimarra' uguale a 0! */
					/* quindi identifichera' lo STANDARD INPUT */

	if (argc <1) /* controllo sul numero di parametri: devono essere in numero minore o uguali a 1! CONTROLLO LASCO! */
        {       printf("Errore: numero di argomenti sbagliato dato che argc = %d\n", argc);
                exit(1); 
	}

    int pid,status;
	for(int i=1;i<argc;i++){
        
        if((pid=fork())<0){
            perror("fork non funzionante");
            exit(1);
        }

        //processo figlio
        if(pid==0){
            execl("/bin/cat","cat",argv[i],(char *)0);
            perror("il cat di  non è stato possibile");
            exit(2);
        }

        if((wait(&status))<0){
            perror("non sono presenti processi figli");
        }
        status=WEXITSTATUS(status);
        
    }


	exit(0);
}