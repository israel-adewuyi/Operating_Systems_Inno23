#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);

    printf("Parent process (PID %d)\n", getpid());

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            return 1;
        } else if (pid == 0) {
            // Child process
            printf("Child process %d (PID %d) created\n", i + 1, getpid());
            sleep(5);
            return 0; // Child process exits
        } else {
            // Parent process continues
            sleep(5);
        }
    }

    // Parent process waits for all child processes to finish
    for (int i = 0; i < n; i++) {
       wait(NULL);
    }

    return 0;
}
