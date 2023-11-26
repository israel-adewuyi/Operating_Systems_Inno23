#include <stdio.h>
#include <errno.h>
#include <linux/input.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define KEYS (1 << 13)

int main(int argc, char** argv) {
        const char* file_path = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
        FILE *file = fopen(file_path, "r");
        
        if (file == NULL) {
                printf("Error opening file\n");
                return 1;
        }
        
        int pressed[KEYS];
        for (int i = 0; i < KEYS; i++) pressed[i] = 0;

        printf("Available Shortcuts:\n");
        printf("P + E: \"I passed the Exam!\"\n");
        printf("C + A + P: \"Get some cappuccino!\"\n");
        printf("C + P: \"Competitive programming!\"\n"); // custom shortcut! 

        struct input_event ev;
        

        /* Read the event
        ssize_t n = read(fd, &ev, sizeof(ev));
        if (n == -1) {
            perror("Error reading event");
            close(fd);
            return 1;
        }
        */
        
        //Endlessly read from file to get all events
        while (1) {
            fread(&ev, sizeof (struct input_event), 1, file);
            if (ev.type == EV_KEY) {
                if (ev.value == 0) {
                    pressed[ev.code] = 0;
                } else if (ev.value == 1) {
                    pressed[ev.code] = 1;
                    if (pressed[25] && pressed[18]) printf("I passed the Exam!\n");
                    else if (pressed[46] && pressed[30] && pressed[25]) printf("Get some cappuccino!\n");
                    else if (pressed[46] && pressed[25]) printf("Competitive programming!\n");
                }
            }
        }
        fclose(file);
        fflush(stdout);

        return 0;
}