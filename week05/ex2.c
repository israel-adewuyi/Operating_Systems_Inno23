#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

#define MAX_LEN 256


struct Thread {
    int id;
    int i;
    char message[MAX_LEN + 1];
};

void *routine(void *args) {
    struct Thread* thread = (struct Thread*) args;
    printf("Thread [id = %d], [message = %s]\n", thread->id, thread->message);
}


int main(int args, char *argv[]){
    printf("Enter the number of threads: ");
    int N;
    scanf("%d", &N);
    struct Thread threads[N];

    for(int i = 0; i < N; i++) {
        threads[i].id = i + 1;
        threads[i].i = i;

        char msg[] = "Hello from thread ";
        char i_str[10];
        sprintf(i_str, "%d", i);
        strcat(msg, i_str);
        strcpy(threads[i].message, msg);
    }

    pthread_t pthreads[N];

    for(int i = 0; i < N; i++){
        if(pthread_create(&pthreads[i], NULL, &routine, &threads[i]) != 0){
            printf("Couldn't create thread %d\n", i);
            return EXIT_FAILURE;
        }

        printf("Thread %d is created\n", i);
    }

    for(int i = 0; i < N; i++){
        if(pthread_join(pthreads[i], NULL));
    }

    return EXIT_SUCCESS;
}