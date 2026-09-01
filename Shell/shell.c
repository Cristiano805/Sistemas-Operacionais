/* minhashell */
#include <stdio.h>
#include <stdlib.h> 
#include <sys/wait.h> 
#include <unistd.h>
#include <string.h>

#define MAX_LINHA 100
#define MAX_ARGS 10

void type_prompt(void) {
    printf("$ ");
    fflush(stdout);
}

void read_command(char *linha, char *argv[]) {    
    char *token;
    int argc = 0;
    int tam;
    if (fgets(linha, MAX_LINHA, stdin) == NULL) {
        return;
    }
    tam = strlen(linha);
    if (tam > 0 && linha[tam - 1] == '\n') {
        linha[tam - 1] = '\0';
    }
    token = strtok(linha, " \t"); 
    while (token != NULL && argc < MAX_ARGS - 1) {
        argv[argc] = token;
        argc++;
        token = strtok(NULL, " \t");
    }
    argv[argc] = NULL; 
}

int main(void) {
    char linha[MAX_LINHA]; 
    char *argv[MAX_ARGS];
    int status;
    while (1) { 
        type_prompt();  
        read_command(linha, argv);
        if (argv[0] == NULL) {
            continue;
        }
        if (strcmp(argv[0], "exit") == 0) {
            break;
        }
        pid_t pid = fork();
        if (pid < 0) {
            perror("Erro ao fazer fork");
        } 
        else if (pid != 0) { 
            waitpid(pid, &status, 0); 
        } 
        else { 
            if (execvp(argv[0], argv) == -1) {
                perror("Erro ao executar comando");
                exit(EXIT_FAILURE); 
            }
        } 
    }
    return 0;
}