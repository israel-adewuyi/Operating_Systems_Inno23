#include <stdio.h>
#include <dirent.h>

int main() {
    // Open the root directory
    DIR *dir = opendir("/");

    // Check if the directory opened successfully
    if (dir == NULL) {
        perror("Error opening root directory");
        return 1;
    }

    // Read the directory entries
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Print the name of each entry
        printf("%s\n", entry->d_name);
    }

    // Close the directory
    closedir(dir);

    return 0;
}
