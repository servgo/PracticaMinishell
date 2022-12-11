#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "parser.h"
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

#define AZUL "\x1b[34m"
#define BLANCO "\x1b[0m"

//Variables globales
tline *line;

//Funciones
void mostrarPrompt();
_Bool commandExists();

int main() {
    char command[1024];
    char *token;
    pid_t pid;

    mostrarPrompt();
    while(fgets(command,1024,stdin)) {
        line = tokenize(command);
        if (line->ncommands==1) {
            pid = fork();
            if (pid<0) {
                fprintf(stderr, "Se ha producido un error al crear el proceso hijo: %s", strerror(errno));
                exit(1);
            } else if (pid==0) { //Proceso hijo
                printf(BLANCO);
                execvp(line->commands[0].argv[0], line->commands[0].argv + 1);
            } else { //Proceso padre
                if (line->background) {

                } else {
                    if (commandExists(line->commands[0].argv)) {
                        waitpid(pid, NULL, 0);
                    } else {
                        fprintf(stderr, "El comando introducido no existe: %s", strerror(errno));
                        exit(1);
                    }
                }
                printf("1== %s",line->commands[0].argv[0]);
                printf("1== %s",line->commands[0].argv[1]);
                printf("1== %s",line->commands[0].argv[2]);
            }
        } else if (line->ncommands>1) {
            printf(BLANCO"Ejecutando mas de 1 comando %s",command);
        }
        mostrarPrompt();
    }
}

void mostrarPrompt() {
    char path[1024];
    getcwd(path,sizeof(path));
    printf(AZUL"%s> ",path);
}

_Bool commandExists(char *command) {
    if (command==NULL) {
        return False;
    } else {
        return True;
    }
}
