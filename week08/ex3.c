#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <unistd.h>

#define MB 1024 * 1024

int main() {
    struct rusage usage;
    int i;
    
    for (i = 0; i < 10; i++) {
        char *ptr = (char *)malloc(100 * MB);
        
        if (ptr == NULL) {
            perror("Memory allocation failed");
            exit(1);
        }
        
        memset(ptr, 0, 100 * MB);
        
        getrusage(RUSAGE_SELF, &usage);
        
        printf("Memory usage (MB): %ld\n", usage.ru_maxrss / 1024);
        
        sleep(1);

        free(ptr);
    }

    
    return 0;
}
