#ifndef _RANDOM_H_
#define _RANDOM_H_

#include <stdint.h>

typedef struct {
  uint32_t seed;
} random_state_t;

/*
 * Generate a random number give a seed state using
 * the Mulberry32 algorithm.
 * 
 * @param state random_state_t containing the random seed
 * @return pseudorandom float on the range [0.0, 1.0)
*/
float random_next(random_state_t *state);

#endif