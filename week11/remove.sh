rm -rf ex1.txt
rm -rf lofsdisk
rm -rf lofs.img

sudo umount ./lofsdisk
sudo losetup -d /dev/loop0
