#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>

#define MAX_LEN (1 << 10)

int main(int argc, char *argv[]){
    int fd[2];

    if(pipe(fd) == -1){
        puts("An error occured while opening pipe");
    }

    pid_t publisher = fork();

    if(publisher == 0){
        char message[MAX_LEN];
        while(true){
            fgets(message, MAX_LEN, stdin);
            write(fd[1], &message, MAX_LEN * sizeof(char));
        }
    }
    else{
        char message[MAX_LEN];
        while(true){
            read(fd[0], &message, MAX_LEN * sizeof(char));
            printf("Message from pushliser: %s\n", message);
        }
    }

    return 0;
}