#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* baseTenToBaseX(long int decimal, int base){
    char* valueRepresentation = NULL;
    int value = 0;
    long int temp = decimal;

    while (temp > 0) {
        temp /= base;
        value++;
    }

    valueRepresentation = (char*)malloc((value + 1) * sizeof(char));
    
    if (valueRepresentation) {
        valueRepresentation[value] = '\0';
        value--;

        while (decimal > 0) {
            int remainder = decimal % base;
            valueRepresentation[value] = '0' + remainder;
            value--;

            decimal /= base;
        }
    }

    return valueRepresentation;
}

char* convert(long int number, int initial_base, int target_base){
    long int decimal = 0l;
    if(initial_base != 10){
        int multiplier = 1;

        while(number > 0){
            int last_digit = number % 10;
            decimal += (last_digit * multiplier);
            multiplier *= initial_base;
            number /= 10;
        }
    }

    printf("Converted number in base 10 is %ld\n", decimal);

    return baseTenToBaseX(decimal, target_base);
}

int main(){
    long int number;
    int initial_base, target_base;

    scanf("%ld %d %d", &number, &initial_base, &target_base);

    if(number < 0 || initial_base < 2 || initial_base > 10 || target_base < 2 || target_base > 10){
        printf("Cannot convert \n");
        return 1;
    }

    printf("%s\n", convert(number, initial_base, target_base));

    printf("Heere are my numbers, %ld, %d, %d \n", number, initial_base, target_base);
    
    return 0;
}