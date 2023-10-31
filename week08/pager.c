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
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <num_pages> <num_frames>\n", argv[0]);
        return 1;
    }

    int num_pages = atoi(argv[1]);
    int num_frames = atoi(argv[2]);

    if (num_frames >= num_pages) {
        fprintf(stderr, "Invalid input: num_frames must be less than num_pages.\n");
        return 1;
    }

    struct PTE* page_table;
    int page_table_fd = open("/tmp/ex2/pagetable", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    size_t page_table_size = sizeof(struct PTE) * num_pages;
    page_table = (struct PTE*) mmap(NULL, page_table_size, PROT_READ | PROT_WRITE, MAP_SHARED, page_table_fd, 0);

    // Set up signal handlers
    struct sigaction sa;
    sa.sa_handler = sigusr1_handler;
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, NULL);

    printf("pager process\n");
    printf("-------------------------\n");

    // Initialize page table
    for (int i = 0; i < num_pages; i++) {
        page_table[i].valid = false;
        page_table[i].frame = -1;
        page_table[i].dirty = false;
        page_table[i].referenced = 0;
    }
    printf("Initialized page table\n");
    for (int i = 0; i < num_pages; i++) {
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i, page_table[i].valid, page_table[i].frame, page_table[i].dirty, page_table[i].referenced);
    }
    printf("-------------------------\n");

    char RAM[num_frames][8];
    char disk[num_pages][8];
    int disk_accesses = 0;

    // Initialize RAM
    for (int i = 0; i < num_frames; i++) {
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
    for (int i = 0; i < num_frames; i++) {
        printf("Frame %d ---> %s\n", i, RAM[i]);
    }
    printf("-------------------------\n");

    printf("Initialized disk\n");
    printf("Disk array\n");
    for (int i = 0; i < num_pages; i++) {
        printf("Page %d ---> %s\n", i, disk[i]);
    }
    printf("-------------------------\n");

    int num_loaded_pages = 0;

    while (num_loaded_pages < num_pages) {
        // Sleep until receiving a SIGUSR1 signal from MMU
        pause();

        int victim_page = -1;

        for (int i = 0; i < num_pages; i++) {
            if (page_table[i].referenced != 0) {
                if (!page_table[i].valid) {
                    if (num_loaded_pages < num_frames) {
                        // Allocate a free frame to the page
                        for (int j = 0; j < num_frames; j++) {
                            if (page_table[i].frame == -1) {
                                page_table[i].frame = j;
                                page_table[i].valid = true;
                                break;
                            }
                        }
                        num_loaded_pages++;
                    } else {
                        // Choose a random victim page
                        victim_page = i;
                    }
                }
                page_table[i].referenced = 0;
            }
        }

        if (victim_page != -1) {
            int victim_frame = rand() % num_frames;
            if (page_table[victim_page].dirty) {
                // Simulate writing the page to disk
                strcpy(disk[victim_page], RAM[victim_frame]);
                disk_accesses++;
            }
            page_table[victim_page].valid = false;
            page_table[victim_page].dirty = false;
            page_table[victim_page].frame = -1;
            printf("A disk access request from MMU Process (pid=%d)\n", getpid());
            printf("Page %d is referenced\n", victim_page);
            printf("We do not have free frames in RAM\n");
            printf("Chose a random victim page %d\n", victim_page);
            printf("Replace/Evict it with page %d to be allocated to frame %d\n", victim_page, victim_frame);
            // Copy data from disk to RAM
            strcpy(RAM[victim_frame], disk[victim_page]);
            printf("RAM array\n");
            for (int i = 0; i < num_frames; i++) {
                printf("Frame %d ---> %s\n", i, RAM[i]);
            }
            printf("disk accesses is %d so far\n", disk_accesses);
            printf("Resume MMU process\n");
            kill(getpid(), SIGCONT);
        }
    }

    printf("%d disk accesses in total\n", disk_accesses);

    // Cleanup and destroy the mapped file
    munmap(page_table, page_table_size);
    close(page_table_fd);
    remove("/tmp/ex2/pagetable");

    printf("Pager is terminated\n");

    return 0;
}
