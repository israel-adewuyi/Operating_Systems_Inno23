#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

//primality test
bool is_prime(int n){
    if (n <= 1)return false;
    int i = 2;
    for(; i * i <= n; i++){
        if(n % i == 0){
            return false;
        }
    }
    return true;
}

//Primes counter in [a, b)
int primes_count(int a, int b){
    int ret = 0;
    for(int i = a; i < b; i++){
        if(is_prime(i) != 0){
            ret++;
        }
    }
    return ret;
}

// argument to the start_routine of the thread
typedef struct prime_request {
    int a, b;
} prime_request ;

// start_routine of the thread
void * prime_counter ( void *arg) {
    // get the request from arg
    prime_request * req = (prime_request*) arg;
    // perform the request
    int * count = malloc(sizeof(int));
    *count = primes_count(req->a, req->b);
    //return count;
    return (( void *) count );
}

int main(int argc, char *argv[]) {
        int n = atoi(argv[1]), n_threads = atoi(argv[2]);
        int segment_size = n / n_threads;

        pthread_t *threads = malloc(n_threads * sizeof(pthread_t));

        prime_request *requests = malloc(n_threads * sizeof(prime_request));

        void **results = malloc(n_threads * sizeof(void *));

        for (int i = 0; i < n_threads; i++) {
                requests[i].a = i * segment_size;
                requests[i].b = (i + 1) * segment_size;
                if (pthread_create(&threads[i], NULL, &prime_counter, &requests[i]) != 0) {
                        printf("Unable to create thread!");
                        return EXIT_FAILURE;
                }
        }


        for (int i = 0; i < n_threads; i++) {
                pthread_join(threads[i], (void**) &results[i]);
        }

        int total_result = 0;
        for (int i = 0; i < n_threads; i++)
                total_result += *(int*) results[i];

        printf("%d\n", total_result);

        free(threads);
        free(results);

        exit(EXIT_SUCCESS);
}