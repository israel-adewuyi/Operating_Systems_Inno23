#!/bin/bash

# Function to retrieve shared libraries associated with a binary
get_libs() {
    # Execute 'ldd' on the specified binary and utilize 'awk' to filter only shared libraries
    ldd "$1" | awk 'BEGIN{ORS=" "}$1~/^\/lib/{print $1}$2~/=>/ && $3~/^\/lib/{print $3}'
}

# Compile ex1.c to generate the executable file named ex1
gcc ex1.c -o ex1

# Create a file with a size of at least 50 MiB
dd if=/dev/zero of=lofs.img bs=1M count=50

# Set up a loop device for the created file
LOOP_DEVICE=$(sudo losetup --find --show lofs.img)
# Retrieve the loop device number (required for association with LOFS)
LOOP_NUMBER=${LOOP_DEVICE#/dev/loop}

# Create a Loop File System (LOFS) with the ext4 format on the previously created file
sudo mkfs.ext4 "$LOOP_DEVICE"

# Create a new empty directory named ./lofsdisk
mkdir -p ./lofsdisk

# Mount the generated filesystem at the mount point ./lofsdisk
sudo mount lofs.img ./lofsdisk

# Add a file named file1 with the content "Israel" to the LOFS
echo "Israel" | sudo tee ./lofsdisk/file1 > /dev/null
# Add a file named file2 with the content "Adewuyi" to the LOFS
echo "Adewuyi" | sudo tee ./lofsdisk/file2 > /dev/null

# Copy the necessary binaries and their shared libraries to the LOFS
for item in bash cat echo ls; do
    # Save the absolute path for the command being processed
    cmd=/bin/$item
    # Output information about the current command being processed
    echo "Processing command: $cmd"
    # Retrieve shared libraries associated with the command
    res=$(get_libs "$cmd")
    for i in $res; do
        # Output information about the current shared library being processed
        echo "Processing library: $i"
        # Create the directory structure in the LOFS for the library
        sudo mkdir -p "./lofsdisk$(dirname $i)"
        # Copy the library to the LOFS
        sudo cp "$i" "./lofsdisk$i"
    done
    # Create the directory structure in the LOFS for the command
    sudo mkdir -p "./lofsdisk$(dirname $cmd)"
    # Copy the command to the LOFS
    sudo cp "$cmd" "./lofsdisk$cmd"
done

# Copy the ex1 executable to the LOFS
sudo cp ex1 ./lofsdisk/ex1

# Add an explanatory line to ex1.txt for human understanding
echo "Output when the root is changed:" > ex1.txt
# Change the root directory of the process to the mount point of the LOFS
sudo chroot ./lofsdisk ./ex1 >> ex1.txt

# Unmount the filesystem
sudo umount ./lofsdisk

# Clean up the loop device
sudo wipefs -a "/dev/loop$LOOP_NUMBER"

# Remove the directory ./lofsdisk
sudo rm -r ./lofsdisk

# Add an explanatory line to ex1.txt for human understanding
echo "\nOutput when the root is unchanged:" >> ex1.txt
# Run ex1 again without changing the root directory
./ex1 >> ex1.txt

