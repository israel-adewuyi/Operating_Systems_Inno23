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
#include <sys/time.h>
#include <limits.h>

#define MAX_PAGES 1000

struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
    int nfu_counter;
    unsigned char aging_counter;
};

int pages;
int frames;
int page_table_size;
struct PTE *pt;
int disk_accesses = 0;
char *RAM[MAX_PAGES];
char *disk[MAX_PAGES];
int free_frames[MAX_PAGES];
char *algorithm;


int my_random(struct PTE* page_table) {
    int random_frame = rand() % frames;
    printf("Choose %d randomly for replacement\n", random_frame);
    for (int page = 0; page < pages; page++) {
        if (page_table[page].frame == random_frame) {
            return page;
        }
    }
}

int nfu(struct PTE* page_table) {
    int min_counter = INT_MAX;
    int page_to_evict = -1;

    for (int page = 0; page < pages; page++) {
        if (page_table[page].valid) {
            if (page_table[page].nfu_counter < min_counter) {
                min_counter = page_table[page].nfu_counter;
                page_to_evict = page;
            }
        }
    }
    printf("NFU page replacement algorithm chose victim page %d for replacement\n",
           page_to_evict);
    return page_to_evict;
}

int aging(struct PTE* page_table) {
    int victim = -1;
    unsigned char min_counter = 255;

    for (int page = 0; page < pages; page++) {
        if (page_table[page].valid) {
            if (page_table[page].aging_counter < min_counter) {
                min_counter = page_table[page].aging_counter;
                victim = page;
                if (min_counter == 0) {
                    break;
                }
            }
        }
    }
    printf("Aging page replacement algorithm chose victim page %d for replacement\n",
           victim);
    return victim;
}

void handle_sigusr1(int signum) {
    int victim = -1;
    for (int i = 0; i < pages; i++) {
        if (pt[i].referenced != 0) {
            victim = i;
            break;
        }
    }

    if (victim != -1) {

        printf("A disk access request from MMU Process (pid=%d)\n",
               pt[victim].referenced);
        int mmu_id = pt[victim].referenced;
        printf("Page %d is referenced\n", victim);

        int free_frame_idx = -1;
        for (int i = 0; i < frames; i ++) {
            if (free_frames[i] != -1) {
                free_frame_idx = i;
                break;
            }
        }
        

        if (free_frame_idx != -1) {
            printf("We can allocate it to free frame %d\n", free_frame_idx);
            pt[victim].valid = true;
            pt[victim].frame = free_frame_idx;
            pt[victim].dirty = false;
            pt[victim].referenced = 0;
            free_frames[free_frame_idx] = -1;

            printf("Copy data from the disk (page=%d) to RAM (frame=%d)\n",
                   victim, free_frame_idx);
            strcpy(RAM[free_frame_idx], disk[victim]);

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

        int evict_page;
        if (strcmp(algorithm, "nfu") == 0) {
            evict_page = nfu(pt);
        } else if (strcmp(algorithm, "aging") == 0) {
            evict_page = aging(pt);
        } else if (strcmp(algorithm, "random") == 0) {
            evict_page = my_random(pt);
        }

        printf("Replace/Evict it with page %d to be allocated to frame %d\n",
               victim, pt[evict_page].frame);

        if (pt[evict_page].dirty) {
            printf("Victim frame %d is dirty. Writing to disk.\n", pt[evict_page].frame);
            disk_accesses++;
            strcpy(disk[evict_page], RAM[pt[evict_page].frame]);
        }
        printf("Copy data from the disk (page=%d) to RAM (frame=%d)\n",
               victim, pt[evict_page].frame);
        strcpy(RAM[pt[evict_page].frame], disk[victim]);

       
        pt[victim].valid = true;
        pt[victim].frame = pt[evict_page].frame;
        pt[victim].dirty = false;
        pt[victim].referenced = 0;

        
        pt[evict_page].valid = false;
        pt[evict_page].frame = -1;
        pt[evict_page].dirty = false;
        pt[evict_page].referenced = 0;

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
    } else {
        printf("No pages need loading. Pager terminating. Disk accesses: %d\n", disk_accesses);
        munmap(pt, sizeof(struct PTE) * pages);
        shm_unlink("/tmp/ex2/pagetable");
        exit(0);
    }
}


char generateRandomChar() {
    const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const int charsetSize = sizeof(charset) - 1;

    return charset[rand() % charsetSize];
}

void printBinary(unsigned char value) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        char * format_message = "<number of pages> <number of frames> <algorithm>";
        fprintf(stderr, "Usage: %s %s\n", argv[0], format_message);
        exit(1);
    }

    pages = atoi(argv[1]);
    frames = atoi(argv[2]);
    page_table_size = pages * sizeof(struct PTE);
    algorithm = argv[3];


    if (frames > pages) {
        fprintf(stderr, "Number of frames cannot be greater than the number of pages.\n");
        exit(1);
    }

    if (strcmp(algorithm, "random") == 0) {
        printf("Selected Random page replacement algorithm.\n");
    } else if (strcmp(algorithm, "nfu") == 0) {
        printf("Selected NFU page replacement algorithm.\n");
    } else if (strcmp(algorithm, "aging") == 0) {
        printf("Selected Aging page replacement algorithm.\n");
    } else {
        fprintf(stderr, "Unknown algorithm. Use 'random', 'nfu', or 'aging'.\n");
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

    pt = mmap(NULL, page_table_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (pt == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    printf("-------------------------\n");
    printf("Initialized page table\n");
    for (int i = 0; i < pages; i++) {
        pt[i].valid = false;
        pt[i].frame = -1;
        pt[i].dirty = false;
        pt[i].referenced = 0;
        pt[i].aging_counter = 0;
        pt[i].nfu_counter = 0;

        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d, "
               "nfu_counter=%d, aging_counter=",
               i, pt[i].valid, pt[i].frame, pt[i].dirty, pt[i].referenced,
               pt[i].nfu_counter);
        printBinary(pt[i].aging_counter);
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
    munmap(pt, page_table_size);

    for (int i = 0; i < pages; i++) {
        free(disk[i]);
    }

    for(int i = 0; i < frames; i++) {
        free(RAM[i]);
    }

    return 0;
}