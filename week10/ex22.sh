#!/bin/bash

#-------------- TASK 1 --------------
#Check the inode of the program ex1. Determine the number of blocks that the file has and the size of each block. Print also thetotal size in bytes and permissions of the file.

# Check the inode
ls -i ex1

# Determine the number of blocks and size of each block
stat ex1

# Print the total size in bytes and permissions
ls -l ex1

#-------------- TASK 2 --------------
#Copy the program ex1 to ex2 and check the number of links for the file ex2. Check if they have same i-node numbers. Justify your answer.
# Copy the file
cp ex1 ex2

# Check the number of links for ex2
ls -l ex2

# Check if they have the same inode numbers
ls -i ex1 ex2


#-------------- TASK 3 --------------
#Identify the files who have 3 links in the path /etc by running the following command line:
stat -c "%h - %n" / etc /* | grep ^3
