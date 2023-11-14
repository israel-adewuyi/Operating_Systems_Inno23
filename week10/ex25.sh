#!/bin/bash

# Example 5: File Permissions

# Make an empty file ex5.txt
touch ex5.txt

# Remove write permission for everybody
chmod a-w ex5.txt
# Explanation: 'a-w' removes the write permission for all users (owner, group, and others).

# Display permission details
ls -l ex5.txt
# -r--r--r-- 1 kezouke kezouke 0 ноя 12 13:00 ex5.txt

# Grant all permissions to owner and others (not group)
chmod uo+rwx ex5.txt
# Explanation: 'uo+rwx' adds read, write, and execute permissions to the owner and others.

# Display permission details
ls -l ex5.txt
# -rwxr--rwx 1 kezouke kezouke 0 ноя 12 13:00 ex5.txt

# Make group permissions equal to user permissions
chmod g=u ex5.txt
# Explanation: 'g=u' sets the group permissions equal to the user (owner) permissions.

# Display permission details
ls -l ex5.txt
# -rwxrwxrwx 1 kezouke kezouke 0 ноя 12 13:00 ex5.txt


# What does 660 mean for ex5.txt?
# Explanation: In octal notation, 660 means read and write permissions for the owner and group, and no permissions for others.

# What does 775 mean for ex5.txt?
# Explanation: In octal notation, 775 means read, write, and execute permissions for the owner and group, and read and execute permissions for others.

# What does 777 mean for ex5.txt?
# Explanation: In octal notation, 777 means full read, write, and execute permissions for the owner, group, and others.

# Clean up - remove the created file
rm ex5.txt
