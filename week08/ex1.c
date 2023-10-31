#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <ctype.h>

int main() {
    // Write the process ID to a file
    FILE *file = fopen("/tmp/ex1.pid", "w");
    if (file) {
        fprintf(file, "%d", getpid());
        fclose(file);
    } else {
        perror("Error writing PID file");
        exit(EXIT_FAILURE);
    }

    // Generate a random password
    char password[9];  // 8 characters + null terminator
    int urandom_fd = open("/dev/urandom", O_RDONLY);
    if (urandom_fd == -1) {
        perror("Error opening /dev/urandom");
        exit(1);
    }

    int idx = 0;
    char c;
    while(idx < 8){
        if (read(urandom_fd, &c, 1) == 1) {
            if (isprint(c)) {
                password[idx] = c;
                idx++;
            }
            if(idx == 8)break;
        }
    }
    
    close(urandom_fd);

    password[8] = '\0';  // Null-terminate the string

    // Store the password in memory using mmap
    char *mem_password = mmap(NULL, 14, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (mem_password == MAP_FAILED) {
        perror("Error mapping memory");
        exit(1);
    }
    strcpy(mem_password, "pass:");
    strcat(mem_password, password);

    printf("%s\n", mem_password);

    // Wait in an infinite loop
    while (1) {
        // To terminate the program, you can use Ctrl+C in the terminal.
    }

    // Unmapping the memory and cleaning up is usually a good practice but not necessary for this example.
    // munmap(mem_password, 9);
    return 0;
}
