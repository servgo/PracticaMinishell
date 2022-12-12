#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "parser.h"
#include <stdlib.h>
#include <errno.h>


#define AZUL "\x1b[34m"
#define BLANCO "\x1b[0m"

//Variables globales
tline *line;

//Funciones
void mostrarPrompt();
int commandExists(char *command);
void ejeCd();



int main() {
    char command[1024];
    char *token;
    pid_t pid;

    mostrarPrompt();
    while (fgets(command, 1024, stdin)) {
        line = tokenize(command);

//--------------------------------------------------------------------------
//Si introducen 1 mandato
//--------------------------------------------------------------------------
        if (line->ncommands == 1) {

            //Si el mandato es cd
            if (strcmp(line->commands[0].argv[0], "cd") == 0) {
                ejeCd();

            //Si el mandato es jobs
            } else if (strcmp(line->commands[0].argv[0], "jobs") == 0) {
                printf("jobs");

            //Si el mandato es fg
            } else if (strcmp(line->commands[0].argv[0], "fg") == 0) {
                printf("fg");

            //Si el mandato es otro cualquiera
            } else { //Es otro mandato
                pid = fork();

                if (pid < 0) {
                    fprintf(stderr, "Se ha producido un error al crear el proceso hijo: %s \n", strerror(errno));
                    exit(1);

                } else if (pid == 0) { //Proceso hijo
                    if (commandExists(line->commands[0].filename) == 0) {
                        execvp(line->commands[0].filename, line->commands[0].argv);
                        fprintf(stderr, "Error al ejecutar el mandato \n");
                        exit(1);
                    } else{
                        fprintf(stderr, "El mandato %s no existe... \n", line->commands[0].argv[0]);
                        exit(1);
                    }
                } else { //Proceso padre
                    if (line->background) {

                    } else {
                        waitpid(pid, NULL, 0);
                    }
                }
            }

//Si introduce mas de 1 mandato
        } else if (line->ncommands > 1) {
            printf(BLANCO"Ejecutando mas de 1 comando %s", command);
        }
        mostrarPrompt();
    }
}

void mostrarPrompt() {
    char path[1024];
    getcwd(path, sizeof(path));
    printf(AZUL"msh:~%s> " BLANCO, path);
}

int commandExists(char *command) {
    return command == NULL;
}

void ejeCd(){
    char buf[1024];
    if (line->commands[0].argc == 1){
        chdir(getenv("HOME"));
        printf("Directorio actual modificado a: %s \n", getcwd(buf, 1024));
    } else if (line->commands->argc == 2){
        if (chdir(line->commands[0].argv[1]) == 0){
            chdir(line->commands[0].argv[1]);
            printf("Directorio actual modificado correctamente a: %s \n", getcwd(buf, 1024));
        } else {
            perror("Error al modificar el directorio\n");
        }
    } else {
        printf("Demasiados argumentos");
    }
}