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

#define MAX_PAGES 100

struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
};

struct PTE *page_table;
int num_pages;
int num_frames;
int page_table_size;
int disk_accesses = 0;
int free_frames[MAX_PAGES];
char * disk[MAX_PAGES];
char * RAM[MAX_PAGES];



void sigusr1_handler(int signo) {
    // Scan the page table for non-zero referenced values
    int victim_page = -1;
    for (int i = 0; i < num_pages; i++) {
        if (page_table[i].referenced != 0) {
            victim_page = i;
            break;
        }
    }

    if (victim_page != -1) {

        printf("A disk access request from MMU Process (pid=%d)\n",
            page_table[victim_page].referenced);
        int mmu_id = page_table[victim_page].referenced;
        printf("Page %d is referenced\n", victim_page);

        // Check if there are free frames
        int free_frame_index = -1;
        for (int i = 0; i < num_frames; i ++) {
            if (free_frames[i] != -1) {
                free_frame_index = i;
                break;
            }
        }
        // printf("free frame index: %d\n", free_frame_index);
        if (free_frame_index != -1) {
            printf("We can allocate it to free frame %d\n", free_frame_index);
            // Allocate a free frame to the page
            page_table[victim_page].valid = true;
            page_table[victim_page].frame = free_frame_index;
            page_table[victim_page].dirty = false;
            page_table[victim_page].referenced = 0;
            free_frames[free_frame_index] = -1;

            printf("Copy data from the disk (page=%d) to RAM (frame=%d)\n",
                victim_page, free_frame_index);
            strcpy(RAM[free_frame_index], disk[victim_page]);

            printf("RAM array\n");
            for(int j = 0; j < num_frames; j++) {
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

        int random_frame = rand() % num_frames;
        printf("Chose a random victim page %d for replacement\n", random_frame);
        printf("Replace/Evict it with page %d to be allocated to frame %d\n",
            victim_page, random_frame);

        for (int p = 0; p < num_pages; p++) {
            if (page_table[p].frame == random_frame) {
                // Check if the victim page is dirty
                if (page_table[p].dirty) {
                    // Simulate writing the page to disk
                    printf("Victim frame %d is dirty. Writing to disk.\n", random_frame);
                    // Copy data from RAM to disk
                    // Assuming disk is a global array
                    // Assuming RAM is a global array
                    // Copy RAM data to disk
                    disk_accesses++;
                    strcpy(disk[p], RAM[random_frame]);
                }
                printf("Copy data from the disk (page=%d) to RAM (frame=%d)\n",
                    victim_page, random_frame);
                strcpy(RAM[random_frame], disk[victim_page]);

                // Update page table for victim_page page
                page_table[p].valid = false;
                page_table[p].frame = -1;
                page_table[p].dirty = false;
                page_table[p].referenced = 0;

                // Allocate the frame to the new page
                page_table[victim_page].valid = true;
                page_table[victim_page].frame = random_frame;
                page_table[victim_page].dirty = false;
                page_table[victim_page].referenced = 0;

                printf("RAM array\n");
                for(int j = 0; j < num_frames; j++) {
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
        // No non-zero referenced fields found, terminate the pager
        printf("No pages need loading. Pager terminating. Disk accesses: %d\n", disk_accesses);
        // Unmap the file and delete it
        munmap(page_table, sizeof(struct PTE) * num_pages);
        shm_unlink("/tmp/ex2/pagetable");
        exit(0);
    }

}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <num_pages> <num_frames>\n", argv[0]);
        return 1;
    }

    num_pages = atoi(argv[1]);
    num_frames = atoi(argv[2]);

    if (num_frames >= num_pages) {
        fprintf(stderr, "Invalid input: Number of frames must be less than number of pages.\n");
        return 1;
    }

    int page_table_fd = open("/tmp/ex2/pagetable", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    page_table_size = sizeof(struct PTE) * num_pages;
    page_table = mmap(NULL, page_table_size, PROT_READ | PROT_WRITE, MAP_SHARED, page_table_fd, 0);

    
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

    disk_accesses = 0;

    for (int i = 0; i < num_frames; i++) {
        RAM[i] = malloc(9);
    }


    for (int i = 0; i < num_pages; i++) {
        disk[i] = malloc(9);

        for (int j = 0; j < 8; j++) {
            disk[i][j] = 'a' + (rand() % 26); 
        }
        disk[i][8] = '\0';
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

    signal(SIGUSR1, sigusr1_handler);

    while(1){
        pause();
    }

    int num_loaded_pages = 0;

    printf("%dTotal Disk Accesses\n", disk_accesses);

    munmap(page_table, page_table_size);
    close(page_table_fd);
    remove("/tmp/ex2/pagetable");

    printf("Pager is terminated\n");

    for(int i = 0; i < num_pages; i++){
        free(disk[i]);
    }

    for(int i = 0; i < num_frames; i++){
        free(RAM[i]);
    }

    return 0;
}
