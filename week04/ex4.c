#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024

void execute_command(char *command, int background) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork error");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        char *args[2];
        args[0] = command;
        args[1] = NULL;

        if (execve(command, args, NULL) == -1) {
            perror("Execve error");
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process
        if (!background) {
            int status;
            waitpid(pid, &status, 0);
        }
    }
}

int main() {
    char input[MAX_INPUT_SIZE];

    while (1) {
        printf("Simplistic Shell > ");
        fgets(input, MAX_INPUT_SIZE, stdin);

        // Remove newline character from input
        input[strlen(input) - 1] = '\0';

        // Check if the command should run in the background
        int background = 0;
        if (input[strlen(input) - 1] == '&') {
            background = 1;
            input[strlen(input) - 1] = '\0';
        }

        // Check if the user wants to exit the shell
        if (strcmp(input, "exit") == 0) {
            break;
        }

        // Execute the command
        execute_command(input, background);
    }

    return 0;
}
