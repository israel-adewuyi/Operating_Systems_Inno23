#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

void* aggregate(void *base, size_t size, int N, void * initial_value, void *(*opr)(const void *, const void *, int)){
    if(size == sizeof(int)){
        void *result = initial_value;

        for(int i = 0; i < N; i++){
            result = opr(result, base + (i * size), sizeof(int));
        }

        return result;
    }
    else{
        void *result = initial_value;

        for(int i = 0; i < N; i++){
            result = opr(result, base + (i * size), sizeof(double));
        }

        return result;
    }
}

void * addition(const void * a, const void * b, int size){
    if(size == sizeof(int)){
        int* result = malloc(sizeof(int));

        * result = *(int *)a + *(int *)b;

        return result;
    }
    else{
        double* result = malloc(sizeof(double));

        * result = *(double *)a + *(double *)b;

        return result;
    }
    
}

void * multiplication(const void * a, const void * b, int size){
    if(size == sizeof(int)){
        int* result = malloc(sizeof(int));
        * result = *(int *)a * *(int *)b;
        return result;
    }
    else{
        double* result = malloc(sizeof(double));
        * result = *(double *)a * *(double *)b;
        return result;
    }
    
}

void * maximum_number(const void * a, const void * b, int size){
    if(size == sizeof(int)){
        int * result = malloc(sizeof(int));
        * result = (*(int*)a > *(int*)b) ? *(int*)a : *(int*)b;
        return result;
    }
    else{
        double * result = malloc(sizeof(double));
        * result = (*(double*)a > *(double*)b) ? *(double*)a : *(double*)b;
        return result;
    }
    
}

int main(){
    int integerArray[5] = {11,22,33,44,55};
    int initialValue = 0;
    int initial_multiplication_value = 1;
    int initial_min_value = INT_MIN;

    int * result = aggregate(integerArray, sizeof(int), 5, &initialValue , addition);
    printf("The result of addition of the integer array is : %d\n", *result);

    int * multiplication_result = aggregate(integerArray, sizeof(int),  5, &initial_multiplication_value, multiplication);
    printf("The result of multiplication of the integer array is : %d\n", *multiplication_result);

    int *maxValue = aggregate(integerArray, sizeof(int), 5, &initial_min_value, maximum_number);
    printf("The maximum value in the integer array is : %d\n\n", *maxValue);

    double doubleArray[] = {1.2, 2.3, 3.4, 4.5, 5.6};
    double initial_addition_value = 0.0;
    double initial_multiplication_value_double = 1.0;
    double initial_min_value_double = DBL_MIN;

    double * addition_result_double = aggregate(doubleArray, sizeof(double), 5, &initial_addition_value , addition);
    printf("The result of addition of the double array is : %f\n", *addition_result_double);

    double * multiplication_result_double = aggregate(doubleArray, sizeof(double),  5, &initial_multiplication_value_double, multiplication);
    printf("The result of multiplication of the integer array is : %f\n", *multiplication_result_double);

    double *maxValue_double = aggregate(doubleArray, sizeof(double), 5, &initial_min_value_double, maximum_number);
    printf("The maximum value in the double array is : %f\n", *maxValue_double);

    free(result);
    free(multiplication_result);
    free(maxValue);
    free(addition_result_double);
    free(multiplication_result_double);
    free(maxValue_double);

    return 0;
}