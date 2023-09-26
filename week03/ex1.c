#include <stdio.h>
#include <stdlib.h> 

int const_tri(int * const p, int N){
    for(int i = 3; i < N; i++){
        int temp = *p + *(p + 1) + *(p + 2);

        *p = *(p + 1);
        *(p + 1) = *(p + 2);
        *(p + 2) = temp;
    }

    return *(p + 2);
}

int main(){
    const int x = 1;
    const int * q = &x;

    int * const p = (int *) malloc(sizeof(int) * 3);

    *p = x;
    *(p + 1) = x;
    *(p + 2) = 2 * x;

    printf("Memory address of the cells are : \n%p,\n%p,\n%p\n", p, (p + 1), (p + 2));
    printf("From the addresses printed above, we can see that the difference in the memory locations are increments of 4, so they are contiguous\n");

    /*
        The question did not ask us to prompt the user for any value of N, but to do that, the little
        code section below should be commented out.
    */

    /*
        nt N;
        scanf("%d", &N);
        printf("The %dth Tribonacci number is : %d\n", N, const_tri(p, N));
    */

    printf("The 6th Tribonacci number is : %d\n", const_tri(p, 6));

    free(p);


    return 0;
}