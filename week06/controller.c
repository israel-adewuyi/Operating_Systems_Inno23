#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    int pid;
    char pid_file_path[] = "/tmp/agent.pid";
    FILE *file = fopen(pid_file_path, "r");
    if (file != NULL) {
        if (fscanf(file, "%d", &pid) == 1) {
            printf("Agent found.\n");
            fclose(file);
        } else {
            printf("Error: No valid PID found in /tmp/agent.pid\n");
            fclose(file);
            exit(1);
        }
    } else {
        printf("Error: No agent found.\n");
        exit(1);
    }

    while (1) {
        printf("Choose a command {\"read\", \"exit\", \"stop\", \"continue\"} to send to the agent: ");
        char input[50];
        if (fgets(input, sizeof(input), stdin) != NULL) {
            // Remove newline character from input
            input[strcspn(input, "\n")] = 0;

            if (strcmp(input, "read") == 0) {
                // Send SIGUSR1 to the agent
                kill(pid, SIGUSR1);
            } else if (strcmp(input, "exit") == 0) {
                // Send SIGUSR2 to the agent and exit
                kill(pid, SIGUSR2);
                exit(0);
            } else if (strcmp(input, "stop") == 0) {
                // Send SIGSTOP to the agent
                kill(pid, SIGSTOP);
            } else if (strcmp(input, "continue") == 0) {
                // Send SIGCONT to the agent
                kill(pid, SIGCONT);
            } else {
                printf("Invalid command. Choose from {\"read\", \"exit\", \"stop\", \"continue\"}.\n");
            }
        }
    }

    return 0;
}
