#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <unistd.h>

#define SECONDS 10
#define SIZE 10 * 1024 * 1024

int main() {
    struct rusage usage;
    int i;
    
    for (i = 0; i < SECONDS; i++) {
        char *ptr = (char *)malloc(SIZE);
        
        if (ptr == NULL) {
            perror("Memory allocation failed");
            exit(1);
        }
        
        memset(ptr, 0, SIZE);
        
        if(getrusage(RUSAGE_SELF, &usage) == 0){
            printf("Memory usage : %ld KB\n", usage.ru_maxrss);
        }
        else{
            perror("Failed to get memory usage");
        }
        
        sleep(1);
    }

    
    return 0;
}
