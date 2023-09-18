#include <stdio.h>
#include <string.h>
#include <ctype.h>


int count(char str[], char ch){
    int N = strlen(str);

    int charCount = 0;

    for(int i = 0; i  < N; i++){
        if(str[i] == ch)charCount++;
    }

    return charCount;
}

void countAll(char str[]){
    int N = strlen(str);

    for(int i = 0; i < N; i++){
        int charCount = count(str, str[i]);
        printf("%c : %d\n", str[i], charCount);
    }
}

int main(){
    char str[256];

    scanf("%s", str);

    for(int i = 0; i < strlen(str); i++){
        str[i] = tolower(str[i]);
    }


    countAll(str);

    return 0;
}