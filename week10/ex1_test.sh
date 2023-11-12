#!/bin/bash

# Usage: ./ex1_test.sh <watched_directory>

watched_directory="$1"

# Ensure a directory is provided
if [ -z "$watched_directory" ]; then
    echo "Usage: ./ex1_test.sh <watched_directory>"
    exit 1
fi

# Change to the watched directory
cd "$watched_directory" || exit 1

# Create a file and two hard links
echo "Creating myfile1.txt with 'Hello world.'"
echo "Hello world." > myfile1.txt
ln myfile1.txt myfile11.txt
ln myfile1.txt myfile12.txt

# Find all hard links to myfile1.txt and print their i-nodes, paths, and content
echo "Finding all hard links to myfile1.txt"
ls -i
echo "---"

# Move the file to another folder
echo "Moving myfile1.txt to /tmp/myfile1.txt"
mv myfile1.txt /tmp/myfile1.txt

# Modify the content of myfile11.txt
echo "Modifying myfile11.txt"
echo "Additional text" >> myfile11.txt

# Create a symbolic link to /tmp/myfile1.txt
echo "Creating symbolic link myfile13.txt to /tmp/myfile1.txt"
ln -s /tmp/myfile1.txt myfile13.txt

# Modify the content of /tmp/myfile1.txt
echo "Modifying /tmp/myfile1.txt"
echo "Additional text" >> /tmp/myfile1.txt

# Remove duplicates of hard links to myfile11.txt
echo "Removing duplicates of hard links to myfile11.txt"
rm myfile12.txt

# Print stat info for the kept hard link
echo "Printing stat info for kept hard link myfile11.txt"
stat myfile11.txt

# Check the number of hard links and explain the difference
echo "Checking the number of hard links"
ls -i
