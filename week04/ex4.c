#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

#define MAXN 1024

void runCommand(char *cmd) {
    char *args[MAXN];
    char *token  = strtok(cmd, " ");
    int count = 0;

    while(token != NULL && count < MAXN - 1){
        args[count] = token;
        token = strtok(NULL, " ");
        count++;
    }

    args[count] = NULL;

    pid_t pid = fork();


    if(pid == 0){
        if(execvp(args[0], args) == -1){
            perror("execvp");
            exit(EXIT_FAILURE);
        }
        exit(0);
    }
}

int main() {
    char input[MAXN];

    while (true) {
        printf("Israel's Simplistic Shell >> ");
        fflush(stdout);
        fgets(input, MAXN, stdin);

        input[strlen(input) - 1] = '\0';

        if (strcmp(input, "exit") == 0) {
            printf("This shell is shutting down. Bye!\n");
            break;
        }

        runCommand(input);
    }

    return 0;
}