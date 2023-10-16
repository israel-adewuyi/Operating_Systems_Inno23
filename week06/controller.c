#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int agent_PID;

void handler(int sig) {
    if (sig == SIGINT) {
        kill(agent_PID, SIGTERM);
        printf("Sent SIGTERM to the agent.\n");
    }
}

int main() {
    char pid_file_path[] = "/tmp/agent.pid";
    FILE *file = fopen(pid_file_path, "r");
    if (file != NULL) {
        if (fscanf(file, "%d", &agent_PID) == 1) {
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

    signal(SIGINT, handler);

    while (1) {
        printf("Choose a command {\"read\", \"exit\", \"stop\", \"continue\"} to send to the agent: ");
        char input[50];
        if (fgets(input, sizeof(input), stdin) != NULL) {
            input[strcspn(input, "\n")] = 0;

            if (strcmp(input, "read") == 0) {
                kill(agent_PID, SIGUSR1);
            } else if (strcmp(input, "exit") == 0) {
                kill(agent_PID, SIGUSR2);
                exit(0);
            } else if (strcmp(input, "stop") == 0) {
                kill(agent_PID, SIGSTOP);
            } else if (strcmp(input, "continue") == 0) {
                kill(agent_PID, SIGCONT);
            } else {
                printf("Invalid command. Choose from {\"read\", \"exit\", \"stop\", \"continue\"}.\n");
            }
        }
    }

    return 0;
}
