#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_THREADS 10
#define MAX_RESOURCES 10

pthread_t threads[MAX_THREADS];
pthread_mutex_t mutexes[MAX_RESOURCES];
int resourceStatus[MAX_RESOURCES]; // 0: unlocked, 1: locked
int deadlock = 0;

void checkDeadlock(int threadID, int resourceID) {
    if (deadlock) return;

    // Implementation of the cycle detection algorithm
    // ... (You need to implement this part based on the described algorithm)
}

void lockResource(int threadID, int resourceID) {
    if (pthread_mutex_trylock(&mutexes[resourceID]) == 0) {
        printf("Thread %d locked mutex %d\n", threadID, resourceID);
        resourceStatus[resourceID] = 1;
    } else {
        printf("Thread %d failed to lock mutex %d\n", threadID, resourceID);
        checkDeadlock(threadID, resourceID);
    }
}

void unlockResource(int threadID, int resourceID) {
    pthread_mutex_unlock(&mutexes[resourceID]);
    printf("Thread %d unlocked mutex %d\n", threadID, resourceID);
    resourceStatus[resourceID] = 0;
}

void* startRoutine(void* arg) {
    int threadID = *((int*)arg);

    printf("Thread %d created\n", threadID);

    // Assume there is a list of requests in input.txt
    FILE* file = fopen("input.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int resourceID;
    while (fscanf(file, "%d", &resourceID) != EOF) {
        lockResource(threadID, resourceID);

        // Simulate some work by sleeping for a random duration (0 to 5 seconds)
        int sleepDuration = rand() % 5;
        sleep(sleepDuration);

        unlockResource(threadID, resourceID);
    }

    fclose(file);

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <number_of_resources> <number_of_threads>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int m = atoi(argv[1]); // Number of resources
    int n = atoi(argv[2]); // Number of threads

    // Initialize mutexes and resourceStatus
    for (int i = 0; i < m; i++) {
        pthread_mutex_init(&mutexes[i], NULL);
        resourceStatus[i] = 0; // 0: unlocked
    }

    // Create threads
    for (int i = 0; i < n; i++) {
        int* threadID = malloc(sizeof(int));
        *threadID = i;
        pthread_create(&threads[i], NULL, startRoutine, (void*)threadID);
    }

    // Wait for all threads to finish
    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    // Cleanup
    for (int i = 0; i < m; i++) {
        pthread_mutex_destroy(&mutexes[i]);
    }

    // Output the status of all threads and whether deadlock occurred
    FILE* outputFile = fopen("ex1.txt", "w");
    if (outputFile == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    if (deadlock) {
        fprintf(outputFile, "Deadlock detected\n");
    } else {
        fprintf(outputFile, "No deadlocks\n");
    }

    fclose(outputFile);

    return 0;
}
