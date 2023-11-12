#!/bin/bash

#-------------- TASK 1 --------------
#Check the file types in /dev and /etc
ls -l /dev
ls -l /etc


#-------------- TASK 2 --------------
#Count the number of directories in the folder /etc by running the command line.
ls -l / etc | grep ^d | wc -l
#ANSWER : 130

#-------------- TASK 3&4 --------------
#Write a hello world program ex1.c and check the file type both before and after compilation using file command.
#Result of running ex1.c with "Hello World";
#C source, ASCII text
#Result of running ex1.c with "Привет, мир!”
#C source, Unicode text, UTF-8 text
#Difference is that the text inside the first program is recognized as an ASCII text and then in the second program, the text inside the file is recognized as Unicode text, specifically UTF-8 encoded. UTF_8 is a variable-width character encoding capable of encoding all possible characters
