#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_LEN (1 << 8)

int main(int argc, char *argv[]) {
    char * myfifo = "/tmp/ex1";

    mkfifo(myfifo, 0666);

    char message[MAX_LEN];

    int fd = open(myfifo, O_RDONLY);

	int id = atoi(argv[1]);

	printf("Subscriber %d is here...\n", id);

    while (1) {
            read(fd, message, MAX_LEN);

            printf("%s", message);
            sleep(1);
    }
    close(fd);

    return 0;
}

// mkfifo reference
// https://man7.org/linux/man-pages/man3/mkfifo.3.html