#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "parser.h"

#define AZUL "\x1b[34m"
#define BLANCO "\x1b[0m"

//Variables globales
tline *line;

int main() {
    char command[1024];
    char path[1024];
    char *token;
    pid_t pid;

    getcwd(path,sizeof(path));
    printf(AZUL"%s> ",path);
    while(fgets(command,1024,stdin)) {
        line = tokenize(command);
        if (line->ncommands==1) {
            printf(BLANCO"Ejecutando 1 comando %s",command);
        } else if (line->ncommands>1) {
            printf(BLANCO"Ejecutando mas de 1 comando %s",command);
        }
        getcwd(path,sizeof(path));
        printf(AZUL"%s> ",path);
    }
}