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

/*
* A function to find the inverse square root of a number
*
* @param a - The number to get the inverse square root of
* @return float - The inverse square root of a
*/
float inverse_sqrt(float a);

/*
* A function to get the sign of a number
*
* @param a - The number to get the sign of
* @return int - The sign (1 for pos, -1 for neg, 0 for 0)
*/
int sgn(int a);

/*
* A function to round a number
*
* @param a - The number to round
* @return int - The rounded number
*/
int round(float a);

/*
* A function to get the min of two ints
*
* @param a - int 1
* @param b - int 2
* @return int - the minimum
*/
int min_int(int a, int b);

/*
* A function to get the max of two ints
*
* @param a - int 1
* @param b - int 2
* @return int - the maximum
*/
int max_int(int a, int b);

/*
 * Converts an unsigned integer to a char* from a buffer.
 * 
 * @param val Integer to convert
 * @param buf Buffer to write string to
 * @param buf_len Maximum length of buffer (including null byte)
*/
void uint_to_string(unsigned int val, char *buf, unsigned int buf_len);

/*
* A function to normalize an angle to its equivalent sine value between 0 and pi/2
*
* @param angle - The angle to normalize
* @return float - The angle mapped between 0 and pi/2
*/
static float normalize_angle(float angle);

/*
* A function to approximate sine using Taylor series
*
* @param angle - The angle (in radians)
@ @return float - The sine of the angle
*/
#define HALF_PI 1.5707963267949
float sin(float angle);

#endif
