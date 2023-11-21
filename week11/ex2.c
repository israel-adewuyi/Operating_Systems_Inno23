#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

struct inode {
    char name[16];
    int size;
    int blockPointers[8];
    int used;
};

int create(char name[16], int size) {
    int fd = open("disk0", O_RDWR);
    char buf[1024];
    int i, j;

    // Step 1: Check for sufficient free space on disk
    lseek(fd, 0, SEEK_SET);
    read(fd, buf, 128);
    for (i = 0; i < size; i++) {
        if (buf[i] != 0) {
            printf("error: insufficient free space on disk\n");
            close(fd);
            return -1;
        }
    }

    // Step 2: Find a free inode
    struct inode newInode;
    lseek(fd, 128, SEEK_SET);
    for (i = 0; i < 16; i++) {
        read(fd, &newInode, sizeof(struct inode));
        if (newInode.used == 0) {
            newInode.used = 1;
            strcpy(newInode.name, name);
            newInode.size = size;

            // Step 3: Allocate data blocks
            for (j = 0; j < size; j++) {
                int found = 0;
                for (int k = 0; k < 128; k++) {
                    if (buf[k] == 0) {
                        buf[k] = 1;
                        newInode.blockPointers[j] = k;
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    // Rollback changes if there is insufficient free space
                    printf("error: insufficient free space on disk\n");
                    for (int l = 0; l < j; l++) {
                        buf[newInode.blockPointers[l]] = 0;
                    }
                    newInode.used = 0;
                    lseek(fd, 128 + i * sizeof(struct inode), SEEK_SET);
                    write(fd, &newInode, sizeof(struct inode));
                    close(fd);
                    return -1;
                }
            }

            // Step 4: Write out the inode and free block list to disk
            lseek(fd, 128 + i * sizeof(struct inode), SEEK_SET);
            write(fd, &newInode, sizeof(struct inode));
            lseek(fd, 0, SEEK_SET);
            write(fd, buf, 128);

            close(fd);
            return 0;
        }
    }

    printf("error: no free inode available\n");
    close(fd);
    return -1;
}


int delete(char name[16]) {
    int fd = open("disk0", O_RDWR); // Assume the disk file is named "disk0"
    char buf[1024];
    int i, j;

    // Step 1: Locate the inode for this file
    struct inode delInode;
    lseek(fd, 128, SEEK_SET);
    for (i = 0; i < 16; i++) {
        read(fd, &delInode, sizeof(struct inode));
        if (delInode.used == 1 && strcmp(delInode.name, name) == 0) {
            // Step 2: Free blocks of the file being deleted
            lseek(fd, 0, SEEK_SET);
            read(fd, buf, 128);
            for (j = 0; j < delInode.size; j++) {
                buf[delInode.blockPointers[j]] = 0;
            }
            lseek(fd, 0, SEEK_SET);
            write(fd, buf, 128);

            // Step 3: Mark inode as free
            delInode.used = 0;

            // Step 4: Write out the inode to disk
            lseek(fd, 128 + i * sizeof(struct inode), SEEK_SET);
            write(fd, &delInode, sizeof(struct inode));

            close(fd);
            return 0;
        }
    }

    printf("error: file not found\n");
    close(fd);
    return -1;
}

int ls(void) {
    int fd = open("disk0", O_RDONLY); // Assume the disk file is named "disk0"
    struct inode tempInode;

    // Step 1: Read in each inode and print
    lseek(fd, 128, SEEK_SET);
    for (int i = 0; i < 16; i++) {
        read(fd, &tempInode, sizeof(struct inode));
        if (tempInode.used == 1) {
            printf("File: %s, Size: %d blocks\n", tempInode.name, tempInode.size);
        }
    }

    close(fd);
    return 0;
}

int read_block(char name[16], int blockNum, char buf[1024]) {
    int fd = open("disk0", O_RDONLY); // Assume the disk file is named "disk0"
    struct inode tempInode;

    // Step 1: Locate the inode for this file
    lseek(fd, 128, SEEK_SET);
    for (int i = 0; i < 16; i++) {
        read(fd, &tempInode, sizeof(struct inode));
        if (tempInode.used == 1 && strcmp(tempInode.name, name) == 0) {
            // Step 2: Read in the specified block
            if (blockNum < tempInode.size) {
                int addr = tempInode.blockPointers[blockNum];
                lseek(fd, addr * 1024, SEEK_SET);
                read(fd, buf, 1024);
                close(fd);
                return 0;
            } else {
                printf("error: blockNum exceeds file size\n");
                close(fd);
                return -1;
            }
        }
    }

    printf("error: file not found\n");
    close(fd);
    return -1;
}

int write_block(char name[16], int blockNum, char buf[1024]) {
    int fd = open("disk0", O_RDWR); // Assume the disk file is named "disk0"
    struct inode tempInode;

    // Step 1: Locate the inode for this file
    lseek(fd, 128, SEEK_SET);
    for (int i = 0; i < 16; i++) {
        read(fd, &tempInode, sizeof(struct inode));
        if (tempInode.used == 1 && strcmp(tempInode.name, name) == 0) {
            // Step 2: Write to the specified block
            if (blockNum < tempInode.size) {
                int addr = tempInode.blockPointers[blockNum];
                lseek(fd, addr * 1024, SEEK_SET);
                write(fd, buf, 1024);
                close(fd);
                return 0;
            } else {
                printf("error: blockNum exceeds file size\n");
                close(fd);
                return -1;
            }
        }
    }

    printf("error: file not found\n");
    close(fd);
    return -1;
}

int main(int argc, char *argv[]) {
    FILE *inputFile;
    char op;
    char name[16];
    int size, blockNum;
    char buf[1024];

    if (argc != 2) {
        fprintf(stderr, "usage: %s <inputFileName>\n", argv[0]);
        exit(1);
    }

    // Open the input file
    if ((inputFile = fopen(argv[1], "r")) == NULL) {
        perror("Error opening input file");
        exit(1);
    }

    // Process operations from the input file
    while (fscanf(inputFile, "%c", &op) != EOF) {
        switch (op) {
            case 'C':
                fscanf(inputFile, "%s %d", name, &size);
                create(name, size);
                break;
            case 'D':
                fscanf(inputFile, "%s", name);
                delete(name);
                break;
            case 'L':
                ls();
                break;
            case 'R':
                fscanf(inputFile, "%s %d", name, &blockNum);
                read_block(name, blockNum, buf);
                printf("Read from file %s, block %d: %s\n", name, blockNum, buf);
                break;
            case 'W':
                fscanf(inputFile, "%s %d", name, &blockNum);
                // Generate dummy content for the buffer
                sprintf(buf, "Dummy data for %s, block %d", name, blockNum);
                write_block(name, blockNum, buf);
                printf("Write to file %s, block %d: %s\n", name, blockNum, buf);
                break;
            default:
                break;
        }
    }

    fclose(inputFile);

    return 0;
}
