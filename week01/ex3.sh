mkdir root
date
sleep 3
ls -latr / > root/root.txt
date
sleep 3
mkdir home
date
sleep 3
ls -latr "/home/${USER}" > home/home.txt
