#!/bin/bash

gcc scheduler.c -o scheduler
gcc worker.c -o worker

./scheduler new_data.txt
#./worker
