#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

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


int n = 0;

pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;

int k = 0;
int c = 0;

int get_number_to_check(){
    int ret = k;
    if (k != n){
        k++;
    }
    return ret;
}

void increment_primes() {
    c++;
}

void *check_primes(void *arg) {
    while (1) {
        pthread_mutex_lock(&global_lock);
        int num = get_number_to_check();
        pthread_mutex_unlock(&global_lock);
   
        if (num == n) {
            break;
        }


        if (is_prime(num)) {
            pthread_mutex_lock(&global_lock);
            increment_primes();
            pthread_mutex_unlock(&global_lock);
        }
    }  
}


int main(int argc, char *argv[]) {
    int n_threads = atoi(argv[2]);
    n = atoi(argv[1]);


    pthread_t *threads = malloc(n_threads * sizeof(pthread_t));
    for (int i = 0; i < n_threads; i++) {
        if (pthread_create(&threads[i], NULL, &check_primes, NULL) != 0) {
            printf("Failed to create the thread");
            return EXIT_FAILURE;
        }
    }
    for (int i = 0; i < n_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("%d\n", c);
   
    free(threads);

    exit(EXIT_SUCCESS);
}
