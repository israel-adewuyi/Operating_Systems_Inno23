#!/bin/bash

PID=$(cat /tmp/ex1.pid)

HEAP_INFO=$(grep "[ ]*heap" /proc/$PID/maps)
START_ADDR=$(echo $HEAP_INFO | awk '{print $1}' | cut -d'-' -f1)
END_ADDR=$(echo $HEAP_INFO | awk '{print $1}' | cut -d'-' -f2)

OFFSET=$((0x$START_ADDR))
LENGTH=$((0x$END_ADDR - 0x$START_ADDR))

PASSWORD_HEX_DUMP=$(sudo dd if=/proc/$PID/mem bs=1 skip=$OFFSET count=$LENGTH 2>/dev/null | xxd | grep -A 8 "pass:" | head -9)

PASS_VALUE=$(echo "$PASSWORD_HEX_DUMP" | grep -oP '(?<=pass:).{8}')

ADDR_HEX=$(echo "$PASSWORD_HEX_DUMP" | head -n 1 | awk '{print $1}' | tr -d ':')
ADDR_DEC=$((16#$ADDR_HEX))


ACTUAL_ADDR=$(printf '%x' $(($OFFSET + $ADDR_DEC)))

echo "Password: $PASS_VALUE"
echo "Memory Address: 0x$ACTUAL_ADDR"

sudo kill -SIGKILL $PID
