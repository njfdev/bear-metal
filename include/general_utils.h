#ifndef _GENERAL_UTILS_H_
#define _GENERAL_UTILS_H_

/*
* A function to swap the value of two integers
*
* @param a - A pointer to the first int
* @param b - A pointer to the second int
*/
void swap_ints(int *a, int *b);

/*
* Absolute value function
*
* @param a - The number to take the absolute value of
* @return float - The absolute value of a
*/
float abs(float a);

/*
* A function to get the decimal part of a float
*
* @param a - The number to get the decimal portion of
* @return float - The decimal portion of a
*/
float decimal_part(float a);

#endif
