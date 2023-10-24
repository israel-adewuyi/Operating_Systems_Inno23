#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/mman.h>

int main() {
    // Step 1: Create an empty file text.txt
    
    int fd = open("text.txt", O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("Failed to open text.txt");
        exit(1);
    }


    // Step 2: Open /dev/random and generate the file content
    int dev_random = open("/dev/random", O_RDONLY);
    if (dev_random == -1) {
        perror("Failed to open /dev/random");
        close(fd);
        exit(1);
    }

    char c;
    int char_count = 0;
    int line_length = 0;
    while (char_count < 500 * 1024 * 1024) { // 500 MiB
        if (read(dev_random, &c, 1) == 1) {
            if (isprint(c)) {
                write(fd, &c, 1);
                char_count++;
                line_length++;
                if (line_length >= 1024){
                    // Add a newline character after 1024 characters
                    write(fd, "\n", 1);
                    line_length = 0;
                }
            }
        }
    }

    close(dev_random);

    long page_size = sysconf(_SC_PAGESIZE);
    long chunk_size = 1024 * page_size;

    // Step 3: Map the file in chunks
    off_t file_size = lseek(fd, 0, SEEK_END);
    off_t offset = 0;

    long long int total_count = 0;

    while (offset < file_size) {
        off_t map_size = (file_size - offset > chunk_size) ? chunk_size : (file_size - offset);
        char *file_data = mmap(NULL, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, offset);

        if (file_data == MAP_FAILED) {
            perror("mmap failed");
            close(fd);
            exit(1);
        }

        // Step 4: Count and replace capital letters with lowercase
        int capital_count = 0;
        for (off_t i = 0; i < map_size; i++) {
            if (isupper(file_data[i])) {
                file_data[i] = tolower(file_data[i]);
                capital_count++;
            }
            total_count += capital_count;
        }

        printf("Chunk offset %ld: Capital letters = %d\n", offset, capital_count);

        munmap(file_data, map_size);
        offset += map_size;
    }

    printf("Capital letters = %lld\n", total_count);

    close(fd);

    return 0;
}
