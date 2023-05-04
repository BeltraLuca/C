#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void){
    
    char c;
    printf("\nINSERIRE ZERO PER TERMINARE L'ESECUZIONE\n");
    scanf("%c",&c);

    if(c!='0'){
        execl("./prova", "prova", (char*) 0);
    }
}