gcc publisher.c -o publisher
gcc subscriber.c -o subscriber

for (i = 1; i <= $4; i++) do
	gnome-terminal -- "./subscriber" $i
done
gnome-terminal -- "./publisher" $1

