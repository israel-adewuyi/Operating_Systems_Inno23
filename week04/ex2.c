#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define VECTOR_SIZE 120

// Global variables for vectors and result
int u[VECTOR_SIZE];
int v[VECTOR_SIZE];
int n;
FILE *result_file;

// Function to calculate dot product for a portion of vectors
void calculate_dot_product(int start, int end, int process_id) {
    int partial_result = 0;
    for (int i = start; i < end; i++) {
        partial_result += u[i] * v[i];
    }

    // Write the partial result to the shared file
    fprintf(result_file, "Process %d: %d\n", process_id, partial_result);
    fflush(result_file);
}

int main() {
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    // Initialize random vectors u and v
    srand(time(NULL));
    for (int i = 0; i < VECTOR_SIZE; i++) {
        u[i] = rand() % 100;
        v[i] = rand() % 100;
    }

    int temp_answer = 0;

    for(int i = 0; i < VECTOR_SIZE; i++){
        printf("%d, %d, %d\n", u[i], v[i], u[i] * v[i]);
        temp_answer += (u[i] * v[i]);
    }
    printf("%d\n", temp_answer);

    // Open the result file for writing
    result_file = fopen("temp.txt", "w");
    if (result_file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int elements_per_process = VECTOR_SIZE / n;
    pid_t pid;

    for (int i = 0; i < n; i++) {
        pid = fork();

        if (pid < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { // Child process
            int start = i * elements_per_process;
            int end = (i + 1) * elements_per_process;
            calculate_dot_product(start, end, i);
            exit(EXIT_SUCCESS);
        }
    }

    // Wait for all child processes to finish
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }

    // Close the result file
    fclose(result_file);

    // Calculate the final result by reading from the file
    result_file = fopen("temp.txt", "r");
    if (result_file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int total_result = 0;
    char line[50];
    while (fgets(line, sizeof(line), result_file)) {
        int partial_result;
        sscanf(line, "Process %*d: %d", &partial_result);
        total_result += partial_result;
    }

    // Close the result file
    fclose(result_file);

    printf("Dot product result: %d\n", total_result);

    return 0;
}
