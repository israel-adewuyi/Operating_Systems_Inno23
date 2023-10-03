#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define M 120

int u[M];
int v[M];
FILE *result_file;

void dot_product(int start, int end, int process_id) {
    int partial_result = 0;
    for (int i = start; i < end; i++) {
        partial_result += u[i] * v[i];
    }

    fprintf(result_file, "The result of process %d: %d\n", process_id, partial_result);
    fflush(result_file);
}

int main() {
    int N;

    printf("Enter the number of processes (it should be divisible by 120): ");

    scanf("%d", &N);
    
    for (int i = 0; i < M; i++) {
        u[i] = rand() % 100;
        v[i] = rand() % 100;
    }

    int temp_answer = 0;

    for(int i = 0; i < M; i++){
        //printf("%d, %d, %d\n", u[i], v[i], u[i] * v[i]);
        temp_answer += (u[i] * v[i]);
    }

    printf("The dot product calculated before using array of processes: %d\n", temp_answer);
    /*
        N = 6 is used to run this program and generate the values in temp.txt
    */

    result_file = fopen("temp.txt", "w");

    int batch_size = M / N;
    pid_t pid;

    for (int i = 0; i < N; i++) {
        pid = fork();

        if (pid == 0) {
            int start = i * batch_size;
            int end = (i + 1) * batch_size;
            dot_product(start, end, i);
            exit(EXIT_SUCCESS);
        }
    }
    
    for (int i = 0; i < N; i++) {
        wait(NULL);
    }

    fclose(result_file);

    result_file = fopen("temp.txt", "r");

    int total_result = 0;

    char line[50];

    while (fgets(line, sizeof(line), result_file)) {
        int partial_result;
        sscanf(line, "The result of process %*d: %d", &partial_result);
        total_result += partial_result;
    }

    fclose(result_file);
    printf("The dot product calculated with array of processes: %d\n", total_result);

    return 0;
}
