#include "general_utils.h"

void swap_ints(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

float abs(float a) {
	if (a < 0) {
		return -a;
	}
	return a;
}

float decimal_part(float a) {
	if (a < 0) {
		return -(a - (int)a);
	}
	return a - (int)a; 
}
