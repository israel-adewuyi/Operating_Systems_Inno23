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


struct PTE *page_table;
int num_pages;
int num_frames;
int pager_pid;

bool paused;


void handle_sigcont(int signum) {
    paused = false;
    printf("Received SIGUSR1: MMU resumed by SIGCONT signal from pager\n");
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Not enough arguments!\n"
               "Usage: %s <num_pages> <ref_string> <pager_pid>\n", argv[0]);
        return 1;
    }
    
    signal(SIGCONT, handle_sigcont);

    num_pages = atoi(argv[1]);
    pager_pid = atoi(argv[argc-1]);
    char ref_string[argc-3][10];
    for (int i = 0; i < argc-3; i++) {
        printf("argv: %s\n", argv[i+2]);
        strcpy(ref_string[i], argv[i + 2]);
    }
    num_frames = num_pages; 

   
    while (access("/tmp/ex2/pagetable", F_OK) == -1) {
        // Sleep for a while and keep checking
        usleep(100000); // Sleep for 0.1 seconds
    }

    int page_table_fd = open("/tmp/ex2/pagetable", O_RDWR);
    if (page_table_fd == -1) {
        perror("open");
        return 1;
    }

    long page_table_size = (long) sizeof(struct PTE) * num_pages;
    if (ftruncate(page_table_fd, page_table_size) == -1) {
        perror("ftruncate");
        exit(1);
    }


    page_table = mmap(NULL, page_table_size, PROT_READ | PROT_WRITE, MAP_SHARED, page_table_fd, 0);
    if (page_table == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    printf("MMU Process\n");
    printf("-------------------------\n");
    printf("Initialized page table\n");
    for (int i = 0; i < num_pages; i++) {
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i, page_table[i].valid, page_table[i].frame, page_table[i].dirty, page_table[i].referenced);
    }
    printf("-------------------------\n");

    for (int i = 0; i < argc-3; i++) {
        printf("-------------------------\n");

        char mode = ref_string[i][0];
        int page = atoi(ref_string[i]+1);
        
        if (mode == 'W') {
            printf("Write Request for page %d\n", page);
        } else if (mode == 'R') {
            printf("Read Request for page %d\n", page);
        } else {
            printf("mode - %c\n", mode);
        }

        if (!page_table[page].valid) {
            printf("It is not a valid page --> page fault\nAsk pager to load it from disk (SIGUSR1 signal) and wait\n");
            page_table[page].referenced = getpid();
            paused = true;
            kill(pager_pid, SIGUSR1);
            while(paused);
            printf("continiue\n");
        } else {
            printf("It is a valid page\n");
        }

        if (mode == 'W') {
            printf("It is a write request then set the dirty field\n");
            page_table[page].dirty = true;
        }

        printf("Page table\n");
        for (int i = 0; i < num_pages; i++) {
            printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i, page_table[i].valid, page_table[i].frame, page_table[i].dirty, page_table[i].referenced);
        }
        printf("-------------------------\n");
    }

    kill(pager_pid, SIGUSR1);
    
    munmap(page_table, page_table_size);
    close(page_table_fd);

    printf("Done all requests.\n");
    printf("MMU sends SIGUSR1 to the pager.\n");
    printf("MMU terminates.\n");

    return 0;
}
