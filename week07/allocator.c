#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MEM_SIZE 100

unsigned int memory_array[MEM_SIZE];
/*
    Array is basically an unsigned int array??
    If we find an empty contiguous memory segment of size "size", say from l to r, do we put adrs in all of the enteries from A[l] to A[r]? A
    Also, same during removal, remove from A[l] to A[r]?? 
*/

bool allocate_best_fit(unsigned int adrs, int size){
    int best_size = MEM_SIZE + 1;
    int best_index = -1;
    int current_size = 0;
    int current_start = -1;;

    printf("I got here\n");

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

    printf("%d\n", current_size);

    if(current_size >= size){
        if(current_size < best_size){
            best_size = current_size;
            best_index = current_start;
        }
    }

    printf("%d\n", best_index);

    if(best_index != -1){
        for(int idx = best_index; idx < best_index + size; idx++){
            memory_array[idx] = adrs;
        }
        return true;
    }
    return false;
}

bool allocate_worst_fit(unsigned int adrs, int size){
    int worst_size = 0;
    int worst_index = -1;
    int current_size = 0;
    int current_start = -1;;

    //printf("I got here\n");

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

    printf("%d\n", current_size);

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
        return true;
    }
    return false;
}

bool allocate_first_fit(unsigned int adrs, int size){
    int j;

    for(int i = 0; i <= MEM_SIZE; i++){
        int j  = i;
        while(j < MEM_SIZE && memory_array[j] == 0){
            j++;
        }
        if(j - i + 1 >= size){
            for (int idx = i; idx < i + size; idx++) {
                memory_array[idx] = adrs;
            }
            return true;
        }
        i = j;
    }
    return false;
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
    
    int total_queries = 0;
    int total_time = 0;
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
            int start_time = clock();
            if (allocate_worst_fit(adrs, size)) {
                total_queries++;
                total_time += clock() - start_time;
            }
        } else if (strcmp(command, "clear") == 0) {
            int start_time = clock();
            clear(adrs);
            total_queries++;
            total_time += clock() - start_time;
        }
    }

    for(int i = 0; i < MEM_SIZE; i++){
        printf("%d ", memory_array[i]);
    }

    
}