#!/bin/bash
gcc publisher.c -o publisher
gcc subscriber.c -o subscriber

for n in $(seq 1 2); do
  gnome-terminal -- "./subscriber" $n
done

gnome-terminal -- "./publisher" $1


