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

float inverse_sqrt(float a) {
	long i;
	float x2, y;
	float three_halfs = 1.5f;

	x2 = a * 0.5f;
	y = a;
	i = *(long*)&y; // move the bits of the float into a long so we can bit manipulate it
	
	// now we can use the fact that for a float reading it as an int is approx the log of the float which turns inverse square root (^-1/2) into multiplication
	i = 0x5f3759df - (i >> 1); // lovely
	y = *(float*)&i; // read the long bits back into a float

	// now we just do newton's method
	y = y * (three_halfs - (x2 * y * y));
	return y;
}

int sgn(int a) {
	if (a > 0) {
		return 1;
	} else if (a < 0) {
		return - 1;
	} return 0;
}

int round(float a) {
	if (a < 0) {
		return (int)(a - 0.5);
	} else {
		return (int)(a + 0.5);
	}
}

int min_int(int a, int b) {
	return (a > b) ? b : a;
}

int max_int(int a, int b) {
	return (a > b) ? a : b;
}

void uint_to_string(unsigned int val, char *buf, unsigned int buf_len) {
	if (buf_len == 0) return;
	if (buf_len == 1) {
		buf[0] = 0;
		return;
	}

	if (val == 0) {
		buf[0] = '0';
		buf[1] = 0;
		return;
	}

	unsigned int cur = val;
	int i = 0;

	while (cur > 0 && i < buf_len-1) {
		cur /= 10;
		i++;
	}

	buf[i] = 0;
	i--;

	cur = val;

	while (cur > 0 && i >= 0) {
		char cur_digit = (cur%10)+48;
		buf[i] = cur_digit;

		cur /= 10;

		i--;
	}
}