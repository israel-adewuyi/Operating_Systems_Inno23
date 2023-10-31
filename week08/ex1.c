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

    // Generate a random password, "pass" + 8 characters + null terminator
    char * password = malloc(14); 

    int urandom_fd = open("/dev/urandom", O_RDONLY);
    if (urandom_fd == -1) {
        perror("Error opening /dev/urandom");
        exit(1);
    }

    password[0] = 'p';
    password[1] = 'a';
    password[2] = 's';
    password[3] = 's';
    password[4] = ':';

    int idx = 5;
    char c;
    while(idx < 13){
        if (read(urandom_fd, &c, 1) == 1) {
            if (isprint(c)) {
                password[idx] = c;
                idx++;
            }
            if(idx == 13)break;
        }
    }
    
    close(urandom_fd);

    password[13] = '\0';  // Null-terminate the string

    printf("%s\n", password);

    // Wait in an infinite loop
    while (1) {
        // To terminate the program, you can use Ctrl+C in the terminal.
    }

    // Unmapping the memory and cleaning up is usually a good practice but not necessary for this example.
    // munmap(mem_password, 9);
    return 0;
}
