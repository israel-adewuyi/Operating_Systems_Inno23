#!/bin/bash

# Read the PID from the file
PID=$(cat /tmp/ex1.pid)

# Extract the start and end addresses of the mapped region
MAPS_ENTRY=$(grep -m 1 "pass:" /proc/$PID/maps)
START_ADDRESS=$(echo $MAPS_ENTRY | awk '{print $1}')
END_ADDRESS=$(echo $MAPS_ENTRY | awk '{print $2}')

# Append "0x" to the start address
MAPPED_PASSWORD_ADDR="0x$START_ADDRESS"

# Use xxd to read the password from memory
#PASSWORD=$(sudo xxd -l 9 -p /proc/$PID/mem -s $MAPPED_PASSWORD_ADDR)

# Print the password and its memory address
#echo "Password: $PASSWORD"
echo "Memory Address: $START_ADDRESS"
echo "Process: $PID"

# Send SIGKILL to the process
sudo kill -9 $PID

