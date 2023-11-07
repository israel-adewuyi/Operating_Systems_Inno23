#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>

struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
    int counter_for_nfu;
    unsigned char counter_for_aging;
};

struct PTE *page_table;
int num_pages;
int num_frames;
int pid_pager;

bool paused;

void handle_sigcont(int signum) {
    paused = false;
    printf("Received SIGUSR1: MMU resumed by SIGCONT signal from pager\n");
}

void printBinary(unsigned char value) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
    printf("\n");
}

void calculate_hit_ratio(int hits_n, int misses_n) {
    float hit_ratio = (float) hits_n / (hits_n + misses_n);
    printf("Hit Ratio: %f\n", hit_ratio);
}

int hits = 0;
int misses = 0;

int main(int argc, char *argv[]) {

    if (argc != 4) {
        printf("Not enough arguments!\n"
               "Usage: %s <num_pages> <reference_string> <pid_pager>\n", argv[0]);
        return 1;
    }

    signal(SIGCONT, handle_sigcont);

    num_pages = atoi(argv[1]);
    char* reference_string = argv[2];
    pid_pager = atoi(argv[3]);
    num_frames = num_pages; 

    while (access("/tmp/ex2/pagetable", F_OK) == -1) {
        usleep(100000); 
    }

    
    int fd = open("/tmp/ex2/pagetable", O_RDWR);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    long page_table_size = (long) sizeof(struct PTE) * num_pages;
    if (ftruncate(fd, page_table_size) == -1) {
        perror("ftruncate");
        exit(1);
    }

    page_table = mmap(NULL, page_table_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (page_table == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    printf("-------------------------\n");
    printf("Initialized page table\n");
    for (int i = 0; i < num_pages; i++) {
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d, "
               "counter_for_nfu=%d, counter_for_aging=",
               i, page_table[i].valid, page_table[i].frame, page_table[i].dirty,
               page_table[i].referenced, page_table[i].counter_for_nfu);
        printBinary(page_table[i].counter_for_aging);
    }
    printf("-------------------------\n");

    char mode;
    int page;
    for (int i = 0; i < strlen(reference_string); i++) {
        sscanf(&reference_string[i], "%c%d", &mode, &page);
        
        printf("-------------------------\n");
        if (mode == 'W') {
            printf("Write Request for page %d\n", page);
        } else if (mode == 'R') {
            printf("Read Request for page %d\n", page);
        } else {
            printf("mode - %c\n", mode);
            continue;
        }

        page_table[page].counter_for_nfu += 1;


        page_table[page].counter_for_aging =
                (page_table[page].counter_for_aging >> 1) | (0x80);
        for (int p = 0; p < num_pages; p++) {
            if (p != page) {
                page_table[p].counter_for_aging =
                        page_table[p].counter_for_aging >> 1;
            }
        }

        if (!page_table[page].valid) {
            misses++;
            printf("It is not a valid page --> page fault\n"
                   "Ask pager to load it from disk (SIGUSR1 signal) and wait\n");
            page_table[page].referenced = getpid();
            paused = true;
            kill(pid_pager, SIGUSR1);
            while (paused);
            printf("continiue\n");
        } else {
            hits++;
            printf("It is a valid page\n");
        }

        if (mode == 'W') {
            printf("It is a write request then set the dirty field\n");
            page_table[page].dirty = true;
        }

        printf("Page table\n");
        for (int i = 0; i < num_pages; i++) {
            printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d, "
                   "counter_for_nfu=%d, counter_for_aging=",
                   i, page_table[i].valid, page_table[i].frame, page_table[i].dirty,
                   page_table[i].referenced, page_table[i].counter_for_nfu);
            printBinary(page_table[i].counter_for_aging);
        }
        printf("\n");
    }
    
    printf("misses: %d\nhits: %d\n", misses, hits);


    printf("Done all requests.\n");
    calculate_hit_ratio(hits, misses);
    printf("MMU sends SIGUSR1 to the pager.\n"
           "MMU terminates.");

    kill(pid_pager, SIGUSR1);
    munmap(page_table, sizeof(struct PTE) * num_pages);
    close(fd);

    return 0;
}