#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/inotify.h>
#include <sys/stat.h>

//#define EVENT_SIZE  ( sizeof (struct inotify_event) )
//#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

char *path;

#define BUF_LEN (1024 * (sizeof(struct inotify_event) + 16))

void print_stat(const char *cur_path) {
    struct stat st;
    if (stat(cur_path, &st) == 0) {
        printf("Stat info for %s:\n", cur_path);
        printf(" - Inode: %lu\n", st.st_ino);
        // Add more stat information as needed
    } else {
        perror("stat");
    }
}

void print_all_stats(const char *dir_path) {
    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char full_path[PATH_MAX];
            snprintf(full_path, PATH_MAX, "%s/%s", dir_path, entry->d_name);
            print_stat(full_path);
        }
    }

    closedir(dir);
}

void handle_event(struct inotify_event *event, const char *cur_path) {
    printf("%s is ", event->name);

    if (event->mask & IN_CREATE) {
        printf("created.\n");
    } else if (event->mask & IN_DELETE) {
        printf("deleted.\n");
    } else if (event->mask & IN_MODIFY) {
        printf("modified.\n");
    } else if (event->mask & IN_ATTRIB) {
        printf("metadata changed.\n");
    } else if (event->mask & IN_ACCESS) {
        printf("accessed.\n");
    } else if (event->mask & IN_OPEN) {
        printf("opened.\n");
    }

    char full_path[PATH_MAX];
    snprintf(full_path, PATH_MAX, "%s/%s", cur_path, event->name);
    print_stat(full_path);
}

void handle_sigint(int signum) {
    // Handle SIGINT (Ctrl+C)
    printf("\nReceived SIGINT. Printing stat info for all entries before termination:\n");
    print_all_stats(path);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    path = argv[1];

    //Create an instance of INOTIFY and check for errors
    int inotify_fd = inotify_init();
    if (inotify_fd == -1) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    /*adding the directory into watch list. Here, the suggestion is to validate the existence of the directory before adding into monitoring list.*/
    int watch_desc = inotify_add_watch(inotify_fd, path, IN_CREATE | IN_DELETE | IN_MODIFY | IN_ATTRIB | IN_ACCESS | IN_OPEN);
    if (watch_desc == -1) {
        perror("inotify_add_watch");
        exit(EXIT_FAILURE);
    }

    printf("Monitoring %s for changes...\n", path);

    // Read events
    char buf[BUF_LEN];
    ssize_t num_read;

    while ((num_read = read(inotify_fd, buf, BUF_LEN)) > 0) {
        for (char *p = buf; p < buf + num_read;) {
            struct inotify_event *event = (struct inotify_event *)p;
            handle_event(event, path);
            p += sizeof(struct inotify_event) + event->len;
        }
    }

    // Clean up
    close(inotify_fd);

    // Print stat info before termination
    printf("Printing stat info for all entries in %s before termination:\n", path);
    // Loop through files and print stat info
    // Add your code here to print stat info for all entries in the directory

    return 0;
}