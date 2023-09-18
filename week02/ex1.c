#include <stdio.h>
#include <limits.h>
#include <float.h>

int main(){
	int int_var = INT_MAX;
	unsigned short int unsigned_short_int_var = USHRT_MAX;
	signed long int signed_long_int_var = LONG_MAX;
	float float_var = FLT_MAX;
	double double_var = DBL_MAX;
	
	printf("Size of integer: %zu bytes\nMaximum number for integer: %d \n\n", sizeof(int), int_var);
	
	printf("Size of unsigned short integer: %zu bytes\nMaximum number for unsigned short integer: %hu \n\n", sizeof(unsigned_short_int_var), unsigned_short_int_var);
	
	printf("Size of signed long integer: %zu bytes\nMaximum number for signed long integer: %ld \n\n", sizeof(signed_long_int_var), signed_long_int_var);
	
	printf("Size of float: %zu bytes\nMaximum number for float: %f \n\n", sizeof(float_var), float_var);
	
	printf("Size of double: %zu bytes\nMaximum number for double: %lf \n\n", sizeof(double_var), double_var);
	

}
