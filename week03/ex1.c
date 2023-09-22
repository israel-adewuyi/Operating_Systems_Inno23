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

    printf("%p , %p , %p , %d, %d, %d\n", p, (p + 1), (p + 2) , *p, *(p + 1), *(p + 2));

    printf("%d \n", const_tri(p, 6));

    free(p);


    return 0;
}