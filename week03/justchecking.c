#include <stdio.h>
#include <stdlib.h>

typedef void * (*opr)(const void *, const void *);

void* aggregate(void *base, size_t size, int N, void * initial_value, void *(*opr)(const void *, const void *)){
    void *result = initial_value;

    for(int i = 0; i < N; i++){
        printf("I got here, actually!!!! %d\n", *(int *)result);
        result = opr(result, base + (i * size));
    }

    return result;
}

void * addition(const void * a, const void * b){
    int* result = malloc(sizeof(int));

    * result = *(int *)a + *(int *)b;

    return result;
}


int main(){
    int integerArray[5] = {11,22,33,44,55};
    int initialValue = 0;

    int * result = aggregate(integerArray, sizeof(int), 5, &initialValue , addition);
    printf("%d\n", *result);

    return 0;
}