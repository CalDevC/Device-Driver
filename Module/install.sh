sudo insmod textModder.ko
sudo mknod /dev/textModder c 415 0
sudo chmod 666 /dev/textModder
echo "The textModder device driver has been installed"
