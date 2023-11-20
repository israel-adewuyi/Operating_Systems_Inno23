#!/bin/bash

# Step 1: Create a file lofs.img not less than 50 MiB using dd
sudo dd if=/dev/zero of=lofs.img bs=1M count=50

# Step 2: Setup a loop device on the created file using losetup
sudo losetup -fP lofs.img

losetup -a

# Step 3: Create a Loop File System (LOFS) ext4 on the loop device using mkfs
sudo mkfs.ext4 lofs.img

# Step 4: Create a new empty directory ./lofsdisk
mkdir lofsdisk

# Step 5: Mount the created filesystem on the mount point ./lofsdisk
#sudo umount ./lofsdisk
sudo mount -o loop /dev/loop24 lofsdisk/

# Step 6: Add two files file1, file2 to the LOFS with first and last name
echo "Israel" | sudo tee ./lofsdisk/file1
echo "Adewuyi" | sudo tee ./lofsdisk/file2

# Step 7: Copy ex1.c to lofsdisk directory
sudo cp ex1.c ./lofsdisk


# Step 8: Define a function get_libs() to get shared libraries of a binary
get_libs() {
    ldd $1 | awk '{print $3}' | grep '/' | tr '\n' ' '
}

# Step 9: Use the function to get shared libraries of specified commands and add them to LOFS
libs_bash=$(get_libs $(which bash))
libs_cat=$(get_libs $(which cat))
libs_echo=$(get_libs $(which echo))
libs_ls=$(get_libs $(which ls))

sudo cp $(which bash) $(which cat) $(which echo) $(which ls) ./lofsdisk

sudo cp /lib/x86_64-linux-gnu/{libtinfo.so.6,libc.so.6,libselinux.so.1,libpcre2-8.so.0} lofsdisk/usr/lib/x86_64-linux-gnu/
sudo cp /lib64/ld-linux-x86-64.so.2 lofsdisk/usr/lib64/

# Step 10: Change the root directory of the process to the mount point of the LOFS and run ex1
sudo chroot ./lofsdisk gcc -o /ex1 ex1.c
sudo chroot ./lofsdisk /ex1 > ex1.txt

# Step 11: Run the same program again without changing the root directory and append the output
sudo ./lofsdisk/ex1 >> ex1.txt

# Step 12: Display the difference between the outputs and add it to ex1.txt
diff ex1.txt ex1.txt >> ex1.txt

# Step 13: Cleanup - Unmount the LOFS and remove the loop device
sudo umount ./lofsdisk
sudo losetup -d /dev/loop0

