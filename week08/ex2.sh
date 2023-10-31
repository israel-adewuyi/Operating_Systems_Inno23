#!/bin/bash

# Compile the programs
gcc mmu.c -o mmu -lrt
gcc pager.c -o pager -lrt

# Run pager in the background
./pager 4 2 &
pager_pid=$!

# Run mmu
./mmu 4 R0 R1 W1 R0 R2 W2 R0 R3 W2 $pager_pid pager

# Clean up
wait $pager_pid
