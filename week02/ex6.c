#include <stdio.h>
#include <stdlib.h>

int main(){
    int count = 7;
    for(int i = 0; i < 7; i++){
        for(int j = 0; j < (count - (count - i - 1)); j++){
            printf("*");
        }
        for(int j = 0; j < (count - i); j++){
            printf(" ");
        }

        
        for(int k = 0; k < (4 - abs(4 - i - 1)); k++){
            printf("*");
        }
        for(int k = 0; k < (7 - (4 - abs(4 - i - 1))); k++){
            printf(" ");
        }

        
        for(int l = 0; l < count; l++){
            printf("*");
        }
        
        printf("\n");
    }
    return 0;
}