#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]){
    if (argc != 2) {
        printf("Usage: %s <number>\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]);
    char pipe_path[20];

    for (int i = 1; i <= n; i++) {
        sprintf(pipe_path, "/tmp/ex1/s%d", i);
        mkfifo(pipe_path, 0666);
        if (fork() == 0) {
            int fd = open(pipe_path, O_WRONLY);
            if (fd == -1) {
                perror("open");
                exit(1);
            }

            char message[256];
            while (1) {
                printf("Enter message for subscriber %d: ", i);
                fgets(message, sizeof(message), stdin);
                write(fd, message, strlen(message));
            }
            close(fd);
            exit(0);
        }
    }

    for (int i = 1; i <= n; i++) {
        wait(NULL);
    }

    return 0;
}