gcc -pthread -o ex3 ex3.c

n=10000000

> ex3_res.txt

for m in 1 2 4 10 100
do
	echo "m = $m" >> ex3_res.txt
	{ time ./ex3 $n $m; } 2>> ex3_res.txt
	echo >> ex3_res.txt
done
