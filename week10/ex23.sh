#!/bin/bash

# Example 3: File operations and link tracing

echo "generating..."
# Create ex1.txt using gen.sh
./gen.sh 10 ex1.txt

echo "linking...."
# Link ex1.txt to ex11.txt and ex12.txt
ln ex1.txt ex11.txt
ln ex1.txt ex12.txt


# Compare content of all files
echo "diff between ex1 and ex11"
diff ex1.txt ex11.txt
echo "diff between ex1 and ex12"
diff ex1.txt ex12.txt
<<comment
No output means there are no differences in content, confirming that all three files contain the same data.
comment

echo "inodes number checking.."
# Check i-node numbers and save the output to output.txt
ls -i ex1.txt ex11.txt ex12.txt > output.txt
<<output
536815 ex11.txt
536815 ex12.txt
536815 ex1.txt
output
<<comment
The inode number 536815 is the same for all three files, indicating they are hard links.
comment

echo "disk usage:"
# Check disk usage of ex1.txt
du ex1.txt
# Output is 4 -- represents the disk space used by the file.

echo "linking..."
# Create a hard link ex13.txt for ex1.txt and move it to /tmp
ln ex1.txt ex13.txt

echo "moving..."
mv ex13.txt /tmp

# Trace all links to ex1.txt in the current path
find ./ -inum $(ls -i ex1.txt | awk '{print $1}') 2>/dev/null
<<output
./ex12.txt
./ex11.txt
./ex1.txt
output

# Trace all links to ex1.txt in the root path (/)
sudo find / -inum $(ls -i ex1.txt | awk '{print $1}') 2>/dev/null
<<output
/home/kezouke/week10/ex12.txt
/home/kezouke/week10/ex11.txt
/home/kezouke/week10/ex1.txt
/tmp/ex13.txt
output

<<comment
These commands trace and list all files linked to the inode number 536815.
In the current path (./), it finds ex1.txt, ex11.txt, and ex12.txt. Hard links only from current directory.
In the root path (/), it finds additional link /tmp/. Since it is started from root -> it founds all possible hlinks
comment

# Check the number of hard links of ex1.txt
stat -c "%h" ex1.txt
# number of hlinks is 4
# The output is 4, indicating that there are four hard links pointing to the same inode.

# Remove all links from ex1.txt
find . -inum $(ls -i ex1.txt | awk '{print $1}') -exec rm {} \;

