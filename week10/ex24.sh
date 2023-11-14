#!/bin/bash

# Example 4: Symbolic Links and Folder Operations

# Delete ./tmp folder if it exists and create a symbolic link tmp1 for the folder ./tmp
if [ -d ./tmp ]; then
    rm -r ./tmp
fi
ln -s "$(pwd)/tmp" tmp1

# Run ls -li
echo "ls -li before creating folder tmp:"
ls -li
echo ""
# Explanation: We're first checking the inode details of files in the current directory.
# The ls -li command provides inode numbers, permissions, link counts, owner, group, size, and modification time.

# Create the folder ./tmp
mkdir ./tmp

# Run ls -li. What is the difference? Justify your answer.
echo "ls -li after creating folder tmp:"
ls -li
echo ""
# Explanation: Now, after creating the ./tmp folder, we observe a new entry in the ls -li output --> entry for tmp (it has already 2 hlinks). 
# The inode number for tmp1 remains the same, indicating that tmp1 is a symbolic link.

# Create a file ex1.txt using gen.sh and add it to ./tmp. Check the folder ./tmp1.
echo "generating ex1.txt and moving it to tmp"
./gen.sh 10 ex1.txt
mv ex1.txt ./tmp
echo ""

cd ~/week10/tmp1
echo "ls -li for tmp1:"
ls -li
echo ""
cd ~/week10
# Explanation: We generate ex1.txt using gen.sh, move it to ./tmp, and observe the details of tmp1.
# The ls -li command here displays inode information for tmp1, now pointing to a directory (./tmp).
<<output
ls -li for tmp1:
total 4
537684 -rw-rw-r-- 1 kezouke kezouke 58 ноя 12 12:29 ex1.txt
output

# Create another symbolic link tmp1/tmp2 to the folder ./tmp (symbolic link to itself).
# Use only absolute paths for the source.
echo "creating recursive link tmp2 to itself"
ln -s "$(pwd)/tmp" "$(pwd)/tmp1/tmp2"
echo ""
# Explanation: We're creating a symbolic link, tmp2, within tmp1 that points back to the ./tmp directory.
# The absolute paths ensure correct link creation, reflecting the system's file structure.

# Create another file ex1.txt using the same generator gen.sh and
# add it to the folder ./tmp1/tmp2.
echo "new ex1.txt in the tmp2"
./gen.sh 10 ex1.txt
mv ex1.txt ./tmp1/tmp2
echo ""
echo "ls -li for tmp2:"
cd ~/week10/tmp1/tmp2
ls -li
echo ""
cd ~/week10
# Explanation: We create a new ex1.txt, move it to tmp1/tmp2, and examine the inode details for tmp2.
# ls -li here illustrates the linkage between tmp2 and the contents of ./tmp.

# Check the content of the sub-folders.
echo "ls -R for tmp1:"
ls -R ~/week10/tmp1
# Explanation: ls -R recursively lists the contents of tmp1, showcasing the interconnected structure
# with tmp2 pointing back to ./tmp and its contents.

# Try to access the sub-folders ./tmp1/tmp2/tmp2/tmp2/.... What did you notice?
cd ~/week10/tmp1/tmp2/tmp2/tmp2
# You will notice that you can keep accessing the symbolic link, creating a loop.
# Explanation: Accessing sub-folders illustrates symbolic link traversal, forming a loop back to itself,
# a behavior inherent in symbolic links.

cd ~/week10

echo "delete tmp"
# Delete the folder ./tmp and check the symbolic links again.
rm -r ~/week10/tmp
echo ""
echo "ls -li now:"
ls -li
<<output
ls -li now:
total 20
536935 -rw-rw-r-- 1 kezouke kezouke 5784 ноя 11 14:33 ex1.c
535144 -rwxrwxr-x 1 kezouke kezouke 2055 ноя 11 16:10 ex23.sh
537697 -rw-rw-r-- 1 kezouke kezouke 1669 ноя 12 12:43 ex24.sh
536614 -rwxrwxr-x 1 kezouke kezouke  122 ноя 11 15:44 gen.sh
537588 lrwxrwxrwx 1 kezouke kezouke   24 ноя 12 12:43 tmp1 -> /home/kezouke/week10/tmp
output
echo ""

# Delete all other symbolic links we created
rm -r ~/week10/tmp1
# Removing tmp1 folder, which includes tmp2 => removing all links

# Explanation: After deleting tmp, we examine the ls -li output again.
# tmp1 and tmp2 still exist but point to a non-existent directory, highlighting symbolic link behavior
# when the target is removed. The system retains symbolic links but marks them as broken or dangling.
# Subsequent access attempts will result in an error.
# Cleanup follows with the removal of tmp1 and its contents.
