#!/bin/bash


mkdir tmp

# Store the current working directory in a variable
current_dir=$(pwd)

# Compile monitor.c
gcc -o monitor monitor.c

# Compile ex1.c
gcc -o ex1 ex1.c

# Run monitor.c in the background
./monitor "$current_dir/tmp" &

sleep 1

# Run ex1.c
gnome-terminal -- bash -c "./ex1 $current_dir/tmp; exec bash"

# Change directory to tmp
cd "$current_dir/tmp"

# Run ex1_test.sh
bash "$current_dir/ex1_test.sh"

# Change directory back to the original directory
cd "$current_dir"

# Terminate monitor.c gracefully with Ctrl+C
kill -INT %%

sleep 3

# Clean up compiled files
rm monitor ex1

rm -r tmp
