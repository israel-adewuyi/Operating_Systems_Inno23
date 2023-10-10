gcc -pthread -o ex4 ex4.c

n=10000000

> ex4_res.txt

for m in 1 2 4 10 100
do
    echo "m = $m" >> ex4.txt
    { time ./ex4 $n $m; } 2>> ex4_res.txt
    echo >> ex4.txt
done

