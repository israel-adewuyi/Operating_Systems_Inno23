#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void handler(int sig) {
    if (sig == SIGUSR1) {
        FILE *file = fopen("text.txt", "r");
        if (file != NULL) {
            char buffer[256];
            while (fgets(buffer, sizeof(buffer), file) != NULL) {
                printf("%s", buffer);
            }
            fclose(file);
        } else {
            printf("Error: Failed to read text.txt\n");
        }
    }
    else{
        printf("Program terminating.....\n");
        exit(EXIT_SUCCESS);
    }
}

int main() {
    char pid_file_path[] = "/tmp/agent.pid";
    FILE *pid_file = fopen(pid_file_path, "w");
    if (pid_file != NULL) {
        fprintf(pid_file, "%d", getpid());
        fclose(pid_file);
    } else {
        printf("Error: Could not create PID file\n");
        exit(1);
    }
    

    // Register signal handlers
    signal(SIGUSR1, handler);
    signal(SIGUSR2, SIG_IGN);

    FILE *read_file = fopen("text.txt", "r");
    if (read_file != NULL) {
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), read_file) != NULL) {
            printf("%s", buffer);
        }
        fclose(read_file);
    } else {
        printf("Error: Failed to read text.txt\n");
    }

    while (1) {
        sleep(1); // Sleep indefinitely
    }

    return 0;
}
