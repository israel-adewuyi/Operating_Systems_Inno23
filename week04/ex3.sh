gcc ex3.c -o ex3
./ex3 3 &

parent_pid=$!

for n in {1 2 3}; do
	echo $n
	pstree $parent_pid
	sleep 5
done


./ex3 5 &

parent_pid=$!

for n in {1 2 3 4 5}; do
	echo $n
	pstree $parent_pid
	sleep 5
done

#parent_pid=$!

#echo "PSTREE for n=3 (Parent Process):"
#pstree $parent_pid
#pstree $parent_pid
#pstree $parent_pid


#./ex3 5 &

#parent_pid=$!

#echo "PSTREE for n=5 (Parent Process):"
#sleep 1
#pstree $parent_pid
#sleep 5
#pstree $parent_pid
#sleep 10
#pstree $parent_pid

