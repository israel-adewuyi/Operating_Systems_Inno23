#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>

struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
};

void sigusr1_handler(int signo) {
    // Do nothing, just wake up from sleep
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <num_pages> <reference_string> <pager_pid>\n", argv[0]);
        return 1;
    }

    int num_pages = atoi(argv[1]);
    char* ref_string = argv[2];
    int pager_pid = atoi(argv[3]);

    struct PTE* page_table;
    int page_table_fd = open("/tmp/ex2/pagetable", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    size_t page_table_size = sizeof(struct PTE) * num_pages;
    ftruncate(page_table_fd, page_table_size);
    page_table = (struct PTE*) mmap(NULL, page_table_size, PROT_READ | PROT_WRITE, MAP_SHARED, page_table_fd, 0);

    // Initialize page table
    for (int i = 0; i < num_pages; i++) {
        page_table[i].valid = false;
        page_table[i].frame = -1;
        page_table[i].dirty = false;
        page_table[i].referenced = 0;
    }

    // Set up signal handlers
    struct sigaction sa;
    sa.sa_handler = sigusr1_handler;
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, NULL);

    char RAM[num_pages][8];
    char disk[num_pages][8];
    int disk_accesses = 0;

    printf("mmu process\n");
    printf("-------------------------\n");
    printf("Initialized page table\n");
    for (int i = 0; i < num_pages; i++) {
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i, page_table[i].valid, page_table[i].frame, page_table[i].dirty, page_table[i].referenced);
    }
    printf("-------------------------\n");

    // Initialize RAM
    for (int i = 0; i < num_pages; i++) {
        memset(RAM[i], 0, 8);
    }

    // Initialize disk
    for (int i = 0; i < num_pages; i++) {
        for (int j = 0; j < 8; j++) {
            disk[i][j] = 'a' + (rand() % 26); // Random characters
        }
    }

    printf("Initialized RAM\n");
    printf("RAM array\n");
    for (int i = 0; i < num_pages; i++) {
        printf("Frame %d ---> %s\n", i, RAM[i]);
    }
    printf("-------------------------\n");

    printf("Initialized disk\n");
    printf("Disk array\n");
    for (int i = 0; i < num_pages; i++) {
        printf("Page %d ---> %s\n", i, disk[i]);
    }
    printf("-------------------------\n");

    int ref_length = strlen(ref_string);
    for (int i = 0; i < ref_length; i += 2) {
        char mode = ref_string[i];
        int page = ref_string[i + 1] - '0';

        printf("Read Request for page %d\n", page);

        if (!page_table[page].valid) {
            printf("It is not a valid page --> page fault\n");
            page_table[page].referenced = getpid();
            kill(pager_pid, SIGUSR1);
            pause(); // Sleep until SIGCONT signal is received
            // When resumed, page should be in RAM
        }

        if (mode == 'W') {
            printf("It is a valid page\n");
            page_table[page].dirty = true;
        }

        printf("Page table\n");
        for (int i = 0; i < num_pages; i++) {
            printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i, page_table[i].valid, page_table[i].frame, page_table[i].dirty, page_table[i].referenced);
        }
        printf("-------------------------\n");
    }

    // Send a signal to pager process to indicate completion
    kill(pager_pid, SIGUSR1);
    
    munmap(page_table, page_table_size);
    close(page_table_fd);

    printf("Done all requests.\n");
    printf("MMU sends SIGUSR1 to the pager.\n");
    printf("MMU terminates.\n");

    return 0;
}
