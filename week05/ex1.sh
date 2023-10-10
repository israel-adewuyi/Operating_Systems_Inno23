#!/bin/bash

# Compile the publisher and subscriber programs
gcc -o publisher publisher.c
gcc -o subscriber subscriber.c

# Loop to open terminal windows and run subscribers
for ((i = 1; i <= $1; i++)); do
  gnome-terminal -- "./subscriber" $i
done

# Start the publisher process in a new terminal
gnome-terminal -- "./publisher" $1


