#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "parser.h"
#include <stdlib.h>
#include <errno.h>

#define AZUL "\x1b[34m"
#define BLANCO "\x1b[0m"

//Variables globales
tline *line;

int main() {
    char command[1024];
    char path[1024];
    char *token;
    pid_t pid;

    getcwd(path, sizeof(path));
    printf(AZUL"%s> ", path);
    while (fgets(command, 1024, stdin)) {
        line = tokenize(command);

        if (line->ncommands == 1) { // falta comprobar si el proceso se ha hecho en bg, en ese caso, deberemos comprobarlo para esperar por el hijo o no
            pid = fork();
            if(pid < 0){ //proceso inexistente
                fprintf(stderr, "Se ha producido un error al crear el proceso hijo: %s", strerror(errno));
                exit(1);
            } else if (pid == 0){ //proceso padre

            }


        } else if (line->ncommands > 1) {
            printf(BLANCO"Ejecutando mas de 1 comando %s", command);
        }
        getcwd(path, sizeof(path));
        printf(AZUL"%s> ", path);
    }

}

