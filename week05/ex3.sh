gcc -pthread -o ex3 ex3.c

n=10000000

> ex3.txt

for m in 1 2 4 10 100
do
	echo "m = $m" >> ex3.txt
	{ time ./ex3 $n $m; } 2>> ex3.txt
	echo >> ex3.txt
done
