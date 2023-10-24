#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MEM_SIZE 10000000

unsigned int memory_array[MEM_SIZE];

bool allocate_best_fit(unsigned int adrs, int size){
    int best_size = MEM_SIZE + 1;
    int best_index = -1;
    int current_size = 0;
    int current_start = -1;;

    for(int i = 0; i < MEM_SIZE; i++){
        if(memory_array[i] == 0){
            current_size++;
            if(current_start == -1){
                current_start = i;
            }
        }
        else{
            if(current_size >= size){
                if(current_size < best_size){
                    best_size = current_size;
                    best_index = current_start;
                }
            }
            current_size = 0;
            current_start = -1;
        }
    }

    //printf("%d\n", current_size);

    if(current_size >= size){
        if(current_size < best_size){
            best_size = current_size;
            best_index = current_start;
        }
    }

    //printf("%d\n", best_index);

    if(best_index != -1){
        for(int idx = best_index; idx < best_index + size; idx++){
            memory_array[idx] = adrs;
        }
        return true;
    }
    return false;
}

void allocate_worst_fit(unsigned int adrs, int size){
    int worst_size = 0;
    int worst_index = -1;
    int current_size = 0;
    int current_start = -1;;

    for(int i = 0; i < MEM_SIZE; i++){
        if(memory_array[i] == 0){
            current_size++;
            if(current_start == -1){
                current_start = i;
            }
        }
        else{
            if(current_size >= size){
                if(current_size > worst_size){
                    worst_size = current_size;
                    worst_index = current_start;
                }
            }
            current_size = 0;
            current_start = -1;
        }
    }

    if(current_size >= size){
        if(current_size > worst_size){
            worst_size = current_size;
            worst_index = current_start;
        }
    }

    if(worst_index != -1){
        for(int idx = worst_index; idx < worst_index + size; idx++){
            memory_array[idx] = adrs;
        }
    }
}

void allocate_first_fit(unsigned int adrs, int size){
    int j;

    for(int i = 0; i < MEM_SIZE; i++){
        int j  = i;
        while(j < MEM_SIZE && memory_array[j] == 0){
            j++;
        }
        if(j - i + 1 >= size){
            for (int idx = i; idx < i + size; idx++) {
                memory_array[idx] = adrs;
            }
            break;
        }
        i = j;
    }
}

void clear(unsigned int adrs){
    for(int idx = 0; idx < MEM_SIZE; idx++){
        if(memory_array[idx] == adrs){
            memory_array[idx] = 0;
        }
    }
}

void initialize_memory(){
    memset(memory_array, 0, sizeof(memory_array));
}

int main(){
    //Memory is initialized, for the first algorithm
    initialize_memory();

    //Open the input file for reading queries
    FILE *input_file = fopen("queries.txt", "r");
    if(input_file == NULL){
        printf("Failed to open queries.txt.\n");
        return 0;
    }

    float total_queries = 0;
    double total_time = 0;
    char line[1024];

    while(fgets(line, sizeof(line), input_file)){
        if(strcmp(line, "end\n") == 0){
            break;
        }

        char command[10];
        unsigned int adrs;
        int size;
        sscanf(line, "%s %u %d", command, &adrs, &size);

        if (strcmp(command, "allocate") == 0) {
            double start_time = clock();
            allocate_first_fit(adrs, size);
            total_queries++;
            total_time += clock() - start_time;
        } else if (strcmp(command, "clear") == 0) {
            double start_time = clock();
            clear(adrs);
            total_queries++;
            total_time += clock() - start_time;
        }
    }
    fclose(input_file);

    total_time /= CLOCKS_PER_SEC;

    printf("First Fit Throughput: %.4f queries per second\n", (float)total_queries / total_time);

    initialize_memory();
    
    total_queries = 0;
    total_time = 0;
    line[1024];

    input_file = fopen("queries.txt", "r");
    if(input_file == NULL){
        printf("Failed to open queries.txt.\n");
        return 0;
    }

    while(fgets(line, sizeof(line), input_file)){
        if(strcmp(line, "end\n") == 0){
            break;
        }

        char command[10];
        unsigned int adrs;
        int size;
        sscanf(line, "%s %u %d", command, &adrs, &size);

        if (strcmp(command, "allocate") == 0) {
            double start_time = clock();
            allocate_worst_fit(adrs, size);
            total_queries++;
            total_time += clock() - start_time;
        } else if (strcmp(command, "clear") == 0) {
            double start_time = clock();
            clear(adrs);
            total_queries++;
            total_time += clock() - start_time;
        }
    }

    total_time /= CLOCKS_PER_SEC;

    printf("Worst Fit Throughput: %.4f queries per second\n", (float)total_queries / total_time);

    

    initialize_memory();
    
    total_queries = 0;
    total_time = 0;
    line[1024];

    input_file = fopen("queries.txt", "r");
    if(input_file == NULL){
        printf("Failed to open queries.txt.\n");
        return 0;
    }

    while(fgets(line, sizeof(line), input_file)){
        if(strcmp(line, "end\n") == 0){
            break;
        }

        char command[10];
        unsigned int adrs;
        int size;
        sscanf(line, "%s %u %d", command, &adrs, &size);

        if (strcmp(command, "allocate") == 0) {
            double start_time = clock();
            allocate_best_fit(adrs, size);
            total_queries++;
            total_time += clock() - start_time;
        } else if (strcmp(command, "clear") == 0) {
            double start_time = clock();
            clear(adrs);
            total_queries++;
            total_time += clock() - start_time;
        }
    }

    total_time /= CLOCKS_PER_SEC;

    printf("Best Fit Throughput: %.4f queries per second\n", (float)total_queries / total_time);

    
}