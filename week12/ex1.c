#include <stdio.h>
#include <errno.h>
#include <linux/input.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define KEYS (1 << 13)

/*
    The key idea behind this solution is to map keys on the keyboard to some numerical value. In this solution, this is represented as indices of 
    the pressed array. 

    The mapping of keys to indices in the pressed array is based on the keycodes defined in the Linux kernel's input-event-codes.h header file. This 
    file assigns unique numerical values (keycodes) to each key on the keyboard.

    In the solution provided, the pressed array is used to keep track of whether a particular key is currently pressed. The array is initialized with a size of KEYS 
    (1 << 13), which means it can represent keycodes from 0 to 8191.
*/

int main(int argc, char** argv) {
    // Specify the path to the keyboard input event file
    const char* file_path = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";

    // Open the input event file for reading
    FILE *file = fopen(file_path, "r");

    // Check if the file was opened successfully
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Create an array to keep track of pressed keys
    int pressed[KEYS];
    for (int i = 0; i < KEYS; i++)
        pressed[i] = 0;

    // Display available shortcuts to the user
    printf("Available Shortcuts:\n");
    printf("P + E: \"I passed the Exam!\"\n");
    printf("C + A + P: \"Get some cappuccino!\"\n");
    printf("C + P: \"Competitive programming!\"\n");
    printf("E + X: Terminate\n"); // Termination shortcut

    // Structure to store input events
    struct input_event ev;

    // Endlessly read events from the file
    while (1) {
        // Read one input event from the file
        fread(&ev, sizeof(struct input_event), 1, file);

        // Check if the event type is a key event
        if (ev.type == EV_KEY) {
            // Check if the key is released
            if (ev.value == 0) {
                // Print RELEASED event information
                printf("RELEASED 0x%04x (%d) \n", ev.code, ev.code);
                pressed[ev.code] = 0;  // Mark the key as not pressed
            } 
            // Check if the key is pressed or repeated
            else if (ev.value == 1 || ev.value == 2) { 
                // Print PRESSED or REPEATED event information
                if (ev.value == 1) {
                    printf("PRESSED 0x%04x (%d) \n", ev.code, ev.code);
                } else {
                    printf("REPEATED 0x%04x (%d) \n", ev.code, ev.code);
                }

                pressed[ev.code] = 1;  // Mark the key as pressed

                // Check for predefined shortcuts
                if (pressed[KEY_P] && pressed[KEY_E]) {
                    printf("I passed the Exam!\n");
                } else if (pressed[KEY_C] && pressed[KEY_A] && pressed[KEY_P]) {
                    printf("Get some cappuccino!\n");
                } else if (pressed[KEY_C] && pressed[KEY_P]) {
                    printf("Competitive programming!\n");
                } else if (pressed[KEY_E] && pressed[KEY_X]) {
                    printf("Terminating...\n");
                    break;  // Terminate the program
                }
            }
        }
    }

    // Close the input event file
    fclose(file);
    fflush(stdout);

    return 0;
}
