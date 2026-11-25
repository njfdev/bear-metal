#include <random.h>

float random_next(random_state_t *state) {
  state->seed += 0x6D2B79F5;
  uint32_t num = state->seed;
  num = (num ^ (num >> 15))*(num|1);
  num ^= num + (num ^ (num >> 7))*(num|61);
  return ((num ^ (num >> 14)) >> 0) / 4294967296.0;
}