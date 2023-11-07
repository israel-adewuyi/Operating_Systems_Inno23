#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/types.h>
#include <time.h>

#define MAX_PAGES 1000

struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
};

int pages;
int frames;
int page_table_size;
struct PTE *page_table;
int disk_accesses = 0;
char *RAM[MAX_PAGES];
char *disk[MAX_PAGES];
int free_frames[MAX_PAGES];

void handle_sigusr1(int signum) {
    int victim = -1;
    for (int page = 0; page < pages; page++) {
        if (page_table[page].referenced != 0) {
            victim = page;
            break;
        }
    }

    if (victim != -1) {

        printf("A disk access request from MMU Process (pid=%d)\n",
               page_table[victim].referenced);
        int mmu_id = page_table[victim].referenced;
        printf("Page %d is referenced\n", victim);

        int free_frame_index = -1;
        for (int i = 0; i < frames; i ++) {
            if (free_frames[i] != -1) {
                free_frame_index = i;
                break;
            }
        }
        if (free_frame_index != -1) {
            printf("We can allocate it to free frame %d\n", free_frame_index);
            page_table[victim].valid = true;
            page_table[victim].frame = free_frame_index;
            page_table[victim].dirty = false;
            page_table[victim].referenced = 0;
            free_frames[free_frame_index] = -1;

            printf("Copy data from the disk (page=%d) to RAM (frame=%d)\n",
                   victim, free_frame_index);
            strcpy(RAM[free_frame_index], disk[victim]);

            printf("RAM array\n");
            for(int j = 0; j < frames; j++) {
                printf("Frame %d ---> %s\n", j, RAM[j]);
            }

            disk_accesses++;
            printf("disk accesses is %d so far\n", disk_accesses);

            kill(mmu_id, SIGCONT);
            printf("Resume MMU process\n"
                   "-------------------------\n");
            return;
        }

        printf("We do not have free frames in RAM\n");

        int random_frame = rand() % frames;
        printf("Chose a random victim page %d for replacement\n", random_frame);
        printf("Replace/Evict it with page %d to be allocated to frame %d\n",
               victim, random_frame);

        for (int page = 0; page < pages; page++) {
            if (page_table[page].frame == random_frame) {
                if (page_table[page].dirty) {
                    printf("Victim frame %d is dirty. Writing to disk.\n", random_frame);
                    disk_accesses++;
                    strcpy(disk[page], RAM[random_frame]);
                }
                printf("Copy data from the disk (page=%d) to RAM (frame=%d)\n",
                       victim, random_frame);
                strcpy(RAM[random_frame], disk[victim]);

                page_table[page].valid = false;
                page_table[page].frame = -1;
                page_table[page].dirty = false;
                page_table[page].referenced = 0;

                page_table[victim].valid = true;
                page_table[victim].frame = random_frame;
                page_table[victim].dirty = false;
                page_table[victim].referenced = 0;

                printf("RAM array\n");
                for(int j = 0; j < frames; j++) {
                    printf("Frame %d ---> %s\n", j, RAM[j]);
                }

                disk_accesses++;
                printf("disk accesses is %d so far\n", disk_accesses);

                kill(mmu_id, SIGCONT);
                printf("Resume MMU process\n"
                       "-------------------------\n");
                return;
            }
        }
    } else {
        printf("No pages need loading. Pager terminating. Disk accesses: %d\n", disk_accesses);
        munmap(page_table, sizeof(struct PTE) * pages);
        shm_unlink("/tmp/ex2/pagetable");
        exit(0);
    }
}


char generateRandomChar() {
    const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const int charsetSize = sizeof(charset) - 1;

    return charset[rand() % charsetSize];
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <number of pages> <number of frames>\n", argv[0]);
        exit(1);
    }

    pages = atoi(argv[1]);
    frames = atoi(argv[2]);
    page_table_size = pages * sizeof(struct PTE);


    if (frames > pages) {
        fprintf(stderr, "Number of frames cannot be greater than the number of pages.\n");
        exit(1);
    }

    srand(time(NULL));

    int fd = open("/tmp/ex2/pagetable", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    if (ftruncate(fd, page_table_size) == -1) {
        perror("ftruncate");
        exit(1);
    }

    page_table = mmap(NULL, page_table_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (page_table == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    printf("-------------------------\n");
    printf("Initialized page table\n");
    for (int i = 0; i < pages; i++) {
        page_table[i].valid = false;
        page_table[i].frame = -1;
        page_table[i].dirty = false;
        page_table[i].referenced = 0;

        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n",
               i, page_table[i].valid, page_table[i].frame, page_table[i].dirty, page_table[i].referenced);
    }
    printf("-------------------------\n\n");


    printf("Initialized RAM\n");
    printf("RAM array\n");
    for (int i = 0; i < frames; i++) {
        RAM[i] = malloc(9);
        printf("Frame %d ---> %s\n", i, RAM[i]);
        free_frames[i] = i;
    }
    printf("-------------------------\n\n");


    printf("Initialized disk\n");
    printf("Disk array\n");
    for (int i = 0; i < pages; i++) {
        disk[i] = malloc(9);

        for (int j = 0; j < 8; j++) {
            disk[i][j] = generateRandomChar();
        }
        disk[i][8] = '\0'; 

        printf("Page %d ---> %s\n", i, disk[i]);
    }
    printf("-------------------------\n\n");

    signal(SIGUSR1, handle_sigusr1);

    while (1) {
        pause();
    }

    printf("Total Disk Accesses: %d\n", disk_accesses);

    close(fd);
    munmap(page_table, page_table_size);

    for (int i = 0; i < pages; i++) {
        free(disk[i]);
    }

    for(int i = 0; i < frames; i++) {
        free(RAM[i]);
    }

    return 0;
}