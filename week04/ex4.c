#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAXN (1 << 8)

void runCommand(char * cmd){
    int error_exists = system(cmd);
    if(error_exists){
        printf("An error occured\n");
    }
}

int main(void){
    char cmd[MAXN];

    pid_t main = getpid();
    while(fgets(cmd, MAXN, stdin)){
        if(getpid() == main){
            fork();
        }
        if(getpid() != main){
            runCommand(cmd);
            return EXIT_SUCCESS;
        }
    }

    return EXIT_SUCCESS;
}