#include <stdio.h>
#include <stdbool.h>

int main(){
	char input_str[256];
	
	char curCharacter;
	
	int idx = 0;
	
	while(true){
		curCharacter = getchar();
		if(curCharacter == '.' || curCharacter == '\n'){
			break;
		}
		input_str[idx] = curCharacter;
		idx++;
	}
	
	idx--;
	
	//printf("%d \n", idx);
	printf("\"");
	while(idx >= 0){
		printf("%c", input_str[idx]);
		idx--;
	}
	printf("\"\n");
	
	return 0;
}
