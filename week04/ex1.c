#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

int main(){
    clock_t parent_start_time = clock(), parent_end_time;

    pid_t first_child = fork();
    pid_t second_child;

    clock_t start_time = clock(), end_time;

    if(first_child == 0){
        end_time = clock();
        printf("The ID of the first process is : %d\nThe parent of the first process is :%d\nThe execution time of the first process is %f\n\n", getpid(), getppid(), (double)end_time - start_time * 1000.0 / CLOCKS_PER_SEC);
        exit(EXIT_SUCCESS);
    }

    
    second_child = fork();
    start_time = clock();


    if(second_child == 0){
        end_time = clock();
        printf("The ID of the second process is : %d\nThe parent of the second process is :%d\nThe execution time of the second process is %f\n\n", getpid(), getppid(), (double)end_time - start_time * 1000.0 / CLOCKS_PER_SEC);
        exit(EXIT_SUCCESS);
    }

    wait(NULL);
    wait(NULL);
    /*
        I observed that if I do not use wait function, the instructions for the parent processes are executed
        and printed first, although, I am not sure how that affects the results of the functions, so the wait allows
        the children processes to run and be terminated before other instructions in the parent process are executed.
    */

    parent_end_time = clock();

    printf("The ID of the parent process is : %d\nThe parent of the parent process is :%d\nThe execution time of the parent process is %f\n\n", getpid(), getppid(), (double)parent_end_time - parent_start_time * 1000.0 / CLOCKS_PER_SEC);

    return 0;
}