#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>

void find_all_hlinks(const char *source) {
    struct stat st;
    if (lstat(source, &st) == -1) {
        perror("lstat");
        exit(EXIT_FAILURE);
    }

    DIR *dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            struct stat cur_st;
            if (lstat(entry->d_name, &cur_st) == -1) {
                perror("lstat");
                exit(EXIT_FAILURE);
            }

            if (st.st_ino == cur_st.st_ino) {
                char absolute_path[PATH_MAX];
                realpath(entry->d_name, absolute_path);
                printf("Hard link: %s (Inode: %lu) -> %s\n", entry->d_name, st.st_ino, absolute_path);
            }
        }
    }

    closedir(dir);
}

void unlink_all(const char *source) {
    struct stat st;
    if (lstat(source, &st) == -1) {
        perror("lstat");
        exit(EXIT_FAILURE);
    }

    DIR *dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            struct stat cur_st;
            if (lstat(entry->d_name, &cur_st) == -1) {
                perror("lstat");
                exit(EXIT_FAILURE);
            }

            if (st.st_ino == cur_st.st_ino && strcmp(source, entry->d_name) != 0) {
                unlink(entry->d_name);
            }
        }
    }

    closedir(dir);
}

void create_sym_link(const char *source, const char *link) {
    if (symlink(source, link) == -1) {
        perror("symlink");
        exit(EXIT_FAILURE);
    }

    printf("Symbolic link created: %s -> %s\n", link, source);
}

void print_stat(const char *path) {
    struct stat st;
    if (lstat(path, &st) == -1) {
        perror("lstat");
        exit(EXIT_FAILURE);
    }

    printf("File: %s\n", path);
    printf("Inode: %lu\n", st.st_ino);
    printf("Size: %lld bytes\n", (long long)st.st_size);
    // Add more stat information as needed
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Create a file and two hard links
    int fd = open("myfile1.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    close(fd);

    link("myfile1.txt", "myfile11.txt");
    link("myfile1.txt", "myfile12.txt");

    // Find all hard links to myfile1.txt
    find_all_hlinks("myfile1.txt");

    // Move the file to another folder
    if (rename("myfile1.txt", "/tmp/myfile1.txt") == -1) {
        perror("rename");
        exit(EXIT_FAILURE);
    }

    // Modify the content of myfile11.txt
    fd = open("myfile11.txt", O_WRONLY | O_APPEND);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    write(fd, " Additional text", 17);
    close(fd);

    // Check if monitor.c reported an event for myfile11.txt
    // Provide your answer and justification in ex1.txt

    // Create a symbolic link to /tmp/myfile1.txt
    create_sym_link("/tmp/myfile1.txt", "myfile13.txt");

    // Modify the content of /tmp/myfile1.txt
    fd = open("/tmp/myfile1.txt", O_WRONLY | O_APPEND);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    
    write(fd, " Additional text", 17);
    close(fd);

    // Check if monitor.c reported an event for myfile13.txt
    // Provide your answer and justification in ex1.txt

    // Remove duplicates of hard links to myfile11.txt
    unlink_all("myfile11.txt");

    // Print stat info for the kept hard link
    print_stat("myfile11.txt");

    // Check the number of hard links and explain the difference
    // Provide your answer in ex1.txt

    return 0;
}
