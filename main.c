#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "parser.h"
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include "TList.h"

#define AZUL "\x1b[34m"
#define BLANCO "\x1b[0m"

//Variables globales
tline *line;

//Funciones
void mostrarPrompt();
int commandExists(char *command);
void ejeCd();
int redirInput();
int redirOutput();
int redirError();

int main() {
    char command[1024];

    TLista *backGround = malloc(sizeof(TLista));
    crearListaVacia(backGround);

    mostrarPrompt();
    while (fgets(command, 1024, stdin)) {
        line = tokenize(command);
        pid_t *pids = malloc(sizeof(pid_t) * line->ncommands);


//----------------------------------------------------------------------------------------------------------------------
//                                                Si introduce 1 mandato
//----------------------------------------------------------------------------------------------------------------------


        if (line->ncommands == 1) { //Si introducen 1 mandato
            if (strcmp(line->commands[0].argv[0], "cd") == 0) { //Si el mandato es cd
                ejeCd();
            } else if (strcmp(line->commands[0].argv[0], "jobs") == 0) { //Si el mandato es jobs
                mostrarLista(backGround);
            } else if (strcmp(line->commands[0].argv[0], "fg") == 0) { //Si el mandato es fg
                printf("fg");
            } else { //Si el mandato es otro cualquiera
                pids[0] = fork();

                if (pids[0] < 0) {
                    fprintf(stderr, "Se ha producido un error al crear el proceso hijo: %s \n", strerror(errno));
                    exit(1);
                } else if (pids[0] == 0) { //Proceso hijo
                    if (line->redirect_input) {
                        if (redirInput() != -1) {
                            redirInput();
                        } else {
                            mostrarPrompt();
                            continue;
                        }
                    }
                    if (line->redirect_error) {
                        if (redirError() != -1) {
                            redirError();
                        } else {
                            mostrarPrompt();
                            continue;
                        }
                    }
                    if (line->redirect_output) {
                        if (redirOutput() != -1) {
                            redirOutput();
                        } else {
                            mostrarPrompt();
                            continue;
                        }
                    }
                    if (commandExists(line->commands[0].filename) == 0) {
                        if (line->background) {
                            int devNull = open("/dev/null", O_RDONLY);
                            dup2(devNull, STDIN_FILENO);
                        }
                        execvp(line->commands[0].filename, line->commands[0].argv);
                        fprintf(stderr, "Error al ejecutar el mandato \n");
                        exit(1);
                    } else {
                        fprintf(stderr, "%s: No se encuentra el mandato. \n", line->commands[0].argv[0]);
                        exit(1);
                    }
                } else { //Proceso padre
                    if (line->background) {
                        TElemento e;
                        crearElemento(pids, command, line->ncommands, &e);
                        insertarLista(&e, backGround);
                        printf("[%d] %d \n", longitudLista(backGround), pids[0]);
                    } else {
                        int est;
                        waitpid(pids[0], &est, 0);
                        for (int i = 1; i < longitudLista(backGround) -1; ++i) {
                            int status;
                            pid_t terminado = waitpid(-1, &status, WNOHANG);
                            if(terminado > 0){
                                // eliminarPorPid(status, backGround)
                            }
                        }
                    }
                }
            }

//----------------------------------------------------------------------------------------------------------------------
//                                          Si introduce mas de 1 mandato
//----------------------------------------------------------------------------------------------------------------------


        } else if (line->ncommands > 1) {
            //Creamos las tuberias
            int **pipes;
            pipes = (int **) malloc( (line->ncommands - 1) * sizeof(int*));
            for (int i = 0; i < line->ncommands -1; ++i) {
                pipes[i] = (int *) malloc(2 * sizeof(int*));
                pipe(pipes[i]);
            }
            //Recorremos todos los mandatos introducidos
            for (int i = 0; i < line->ncommands; i++) {
                if (commandExists(line->commands[i].filename) == 0) {
                    pids[i] = fork();
                    if (pids[i] < 0) {
                        fprintf(stderr, "Se produjo un error al crear el proceso hijo.\n");
                        exit(-1);
                    } else if (pids[i] == 0) { //Proceso hijo

                        //Si el proceso es el primero tendremos que redirigir la salida del primer mandato a la salida de la
                        //primera tuberia y cerrar la lectura en ella
                        if (i == 0) {

                            //comprobamos que si hay redireccion de entrada
                            if (line->redirect_input != NULL) {
                                redirInput();
                            }
                            //De momento, la salida de la primera tuberia es nuestra salida estandar
                            dup2(pipes[i][1], 1);


                            //Si es el ultimo proceso :
                        } else if (i == (line->ncommands - 1)) {

                            //comprobamos si hay redireccion de error o de salida
                            if (line->redirect_output != NULL) {
                                redirOutput();
                            }
                            if (line->redirect_error != NULL) {
                                redirError();
                            }

                            //hago que la entrada estandar sea la salida de la anterior
                            dup2(pipes[i - 1][0], 0);

                        } else {

                            //Redirijo la salida anterior a la entrada de la siguiente
                            dup2(pipes[i - 1][0], 0);
                            dup2(pipes[i][1], 1);

                        }
                        //Eierro las tuberias innecesarias
                        for (int j = 0; j < line->ncommands - 1; j++) {
                            close(pipes[j][0]);
                            close(pipes[j][1]);
                        }

                        //Ejecutamos el mandato
                        execv(line->commands[i].filename, line->commands[i].argv);
                        fprintf(stderr, "Error al ejecutar el mandato.\n");
                        exit(1);
                    }
                } else {
                    fprintf(stderr, "%s: No se encuentra el mandato. \n", line->commands[i].argv[0]);
                }
            }
            //Cierro las tuberías en el padre
            for (int j = 0; j < line->ncommands - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
                free(pipes[j]);
            }
            waitpid(pids[line->ncommands - 1], NULL, 0); // 0 para que enlace con cualquier proceso de la misma familia

            free(pipes);
        }

        free(pids);
        mostrarPrompt();
    }

    for (int i = 0; i < longitudLista(backGround) -1; ++i) {
        free(backGround[i]);
    }
    free(backGround);
}

//----------------------------------------------------------------------------------------------------------------------
//                                                     Funciones
//----------------------------------------------------------------------------------------------------------------------

void mostrarPrompt() {
    char path[1024];
    getcwd(path, sizeof(path));
    printf(AZUL"msh:~%s> " BLANCO, path);
}

int commandExists(char *command) {
    return command == NULL;
}

void ejeCd() {
    char buf[1024];
    if (line->commands[0].argc == 1) {
        chdir(getenv("HOME"));
        printf("Directorio actual modificado a: %s \n", getcwd(buf, 1024));
    } else if (line->commands->argc == 2) {
        if (chdir(line->commands[0].argv[1]) == 0) {
            printf("Directorio actual modificado correctamente a: %s \n", getcwd(buf, 1024));
        } else {
            perror("Error al modificar el directorio: ");
        }
    } else {
        printf("Demasiados argumentos");
    }
}

int redirInput() {
    int file = open(line->redirect_input, O_RDONLY);
    if (file != -1) {
        dup2(file, 0);
        return 0;
    } else {
        fprintf(stderr, "%s: Error al abrir el fichero. %s\n", line->redirect_input, strerror(errno));
        return -1;
    }
}

int redirOutput() {
    int file = creat(line->redirect_output, 0664);
    if (file != -1) {
        dup2(file, STDOUT_FILENO);
        return 0;
    } else {
        fprintf(stderr, "%s: Error al abrir el fichero. %s\n",line->redirect_output, strerror(errno));
        return -1;
    }
}

int redirError() {
    int file = creat(line->redirect_error, 0664);
    if (file != -1) {
        dup2(file, 2);
        return 0;
    } else {
        fprintf(stderr, "%s: Error al abrir el fichero. %s\n", line->redirect_error, strerror(errno));
        return -1;
    }
}
