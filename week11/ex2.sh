#!/bin/bash

# Compile create_fs.c
gcc create_fs.c -o create_fs

# Run create_fs to create the initial file system
./create_fs disk0

# Compile ex2.c
gcc ex2.c -o ex2

# Run ex2 with input file input.txt
./ex2 input.txt

# Clean up executable files
rm create_fs ex2

