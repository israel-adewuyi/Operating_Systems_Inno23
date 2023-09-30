#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main(){
    pid_t first_child = fork();
    pid_t second_child;

    clock_t start_time = clock(), end_time;

    if(first_child == 0){
        end_time = clock();
        printf("The ID of the first process is : %d\nThe parent of the first process is :%d\nThe execution time of the first process is %f\n\n", getpid(), getppid(), (double)end_time - start_time * 1000.0 / CLOCKS_PER_SEC);
        exit(0);
    }

    printf("My ID %d, My parent ID %d\n", getpid(), getppid());

    
    second_child = fork();
    start_time = clock();


    if(second_child == 0){
        end_time = clock();
        printf("The ID of the first process is : %d\nThe parent of the first process is :%d\nThe execution time of the first process is %f\n\n", getpid(), getppid(), (double)end_time - start_time * 1000.0 / CLOCKS_PER_SEC);
        exit(0);
    }

    wait(NULL);
    wait(NULL);

    return 0;
}