#!/bin/bash

gcc pager_2.c -o pager_2 
gcc mmu_2.c -o mmu_2

mkdir -p /tmp/ex2

touch /tmp/ex2/pagetable

reference_string="R0 R1 R0 W1 R0 R1 R0 W1 R0 R2 R0 W2 R0 R2 R0 W2 R0 R3 R0 W3 R0 R3 R0 W3 R0 R4 R0 W4 R0 R4 R0 W4"

./pager_2 10 3 &

pid_pager=$!

sleep 3

gnome-terminal -- bash -c "./mmu_2 5 '$reference_string' $pid_pager; exec bash"

rm pager_2 mmu_2

wait

rm -r /tmp/ex2
