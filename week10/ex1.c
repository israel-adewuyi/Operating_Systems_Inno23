#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <dirent.h>

//path to watched directory
char *path;

// Function to find all hard links to a given file

void find_all_hlinks(const char* source) {
    struct stat st;
    if (lstat(source, &st) == -1) {
        perror("lstat");
        exit(EXIT_FAILURE);
    }

    printf("Hard links to %s (inode %lu):\n", source, (unsigned long)st.st_ino);

    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(path)) == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
	char entry_path[PATH_MAX];
    	snprintf(entry_path, sizeof(entry_path), "%s/%s", path, entry->d_name);
        struct stat entry_stat;
        if (lstat(entry_path, &entry_stat) == -1) {
            perror("lstat");
            exit(EXIT_FAILURE);
        }

        if (entry_stat.st_ino == st.st_ino && strcmp(entry_path, source) != 0) {
            printf("Hard link :\n\tInode: %lu, Path: %s\n",
                    (unsigned long)entry_stat.st_ino, entry_path);
        }
    }
    printf("\n");
    closedir(dir);
}

// Function to unlink all duplicates of a hard link, keeping only one
void unlink_all(const char* source) {
    struct stat st;
    if (lstat(source, &st) == -1) {
        perror("lstat");
        exit(EXIT_FAILURE);
    }

    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(path)) == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
    	char entry_path[PATH_MAX];
    	snprintf(entry_path, sizeof(entry_path), "%s/%s", path, entry->d_name);
        
        struct stat entry_stat;
        if (lstat(entry_path, &entry_stat) == -1) {
            perror("lstat");
            exit(EXIT_FAILURE);
        }

        if (entry_stat.st_ino == st.st_ino && strcmp(entry_path, source) != 0) {
            if (unlink(entry_path) == -1) {
                perror("unlink");
                exit(EXIT_FAILURE);
            }
            printf("Successefull unlink happen for: %s\n", entry_path);
        }
    }
    printf("\n");
    closedir(dir);
}

// Function to create a symbolic link to a file in the watched directory
void create_sym_link(const char* source, const char* link) {
    char link_path[PATH_MAX];
    snprintf(link_path, sizeof(link_path), "%s/%s", path, link);

    if (symlink(source, link_path) == -1) {
        perror("symlink");
        exit(EXIT_FAILURE);
    }
    printf("Symbolic link {%s} created for source %s\n\n", link_path, source);
}

void print_stat_info(const char *entry_name) {
    char full_path[PATH_MAX];
    sprintf(full_path, "%s/%s", path, entry_name);

    struct stat st;
    if (stat(full_path, &st) == 0) {
        printf("Stat info for %s:\n", full_path);
        printf("  Size: %ld bytes\n", st.st_size);
        printf("  Inode: %ld\n", st.st_ino);
        printf("  Number of Hard Links: %ld\n", st.st_nlink);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <watched_directory_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    path = argv[1];

    // Create myfile1.txt with content "Hello world."
    char myfile1_path[PATH_MAX];
    snprintf(myfile1_path, sizeof(myfile1_path), "%s/myfile1.txt", path);

    FILE *myfile1 = fopen(myfile1_path, "w");
    if (myfile1 == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
     
    
    fprintf(myfile1, "Hello world.\n");
     
    
    fclose(myfile1);

    // Create hard links to myfile1.txt
    char myfile11_path[PATH_MAX];
    char myfile12_path[PATH_MAX];
    snprintf(myfile11_path, sizeof(myfile11_path),
             "%s/myfile11.txt", path);
    snprintf(myfile12_path, sizeof(myfile12_path),
             "%s/myfile12.txt", path);

    if (link(myfile1_path, myfile11_path) == -1 || link(myfile1_path, myfile12_path) == -1) {
        perror("link");
        exit(EXIT_FAILURE);
    }
    
     

    // Find and print all hard links to myfile1.txt
    find_all_hlinks(myfile1_path);
         

    // Move myfile1.txt to /tmp/myfile1.txt
    if (rename(myfile1_path, "/tmp/myfile1.txt") == -1) {
        perror("rename");
        exit(EXIT_FAILURE);
    }
     

    // Modify myfile11.txt
    FILE *myfile11 = fopen(myfile11_path, "a");
    if (myfile11 == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
     
    
    fprintf(myfile11, "Modified file.");
     
    fclose(myfile11);


    // Create symbolic link myfile13.txt to /tmp/myfile1.txt
    create_sym_link("/tmp/myfile1.txt", "myfile13.txt");

    // Modify /tmp/myfile1.txt
    FILE *tmpfile1 = fopen("/tmp/myfile1.txt", "a");
    if (tmpfile1 == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    
    fprintf(tmpfile1, "/tmp/myfile1.txt has been modified.");
     
    fclose(tmpfile1);

    // Remove duplicates of hard links to myfile11.txt
    unlink_all(myfile11_path);
     

     
    printf("Stat info for the kept hard link:\n");
    print_stat_info("myfile11.txt");
    return 0;
}