#include <stdio.h>

int tribonacci(int N){
    int zerothTerm = 0, firstTerm = 1, secondTerm = 1;

    for(int i = 3; i <= N; i++){
        int newTerm = zerothTerm + firstTerm + secondTerm;

        zerothTerm = firstTerm;
        firstTerm = secondTerm;
        secondTerm = newTerm;
    }

    return secondTerm;
}

int main(){
    int N;
    scanf("%d", &N);

    printf("The nth tribonacci number is %d\n", tribonacci(N));

}