#!/bin/bash

mkdir -p /tmp/ex2
touch /tmp/ex2/pagetable


gcc pager.c -o pager 
gcc mmu.c -o mmu

 
./pager 4 2 &
pid_pager=$!

sleep 3

gnome-terminal -- bash -c "./mmu 4 R0 R1 W1 R0 R2 W2 R0 R3 W2 $pid_pager; exec bash"


rm pager mmu

wait

rm -r /tmp/ex2
