#!/bin/bash

# Script to generate a text file with random numbers

for ((i=0; i<$1; i++)); do
    echo $RANDOM >> $2
done
