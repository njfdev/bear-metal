#include "asm.h"
#include <timer.h>

const unsigned int pit_divisor = 1193182/1000; // triggers at 1000hz

// TODO: set this up with interrupts
void timer_init() {
	// setup 1000 hz timer on channel 0 of PIT
	outb(0x76, 0x43);
	outb(pit_divisor&0xff, 0x40);
	outb(pit_divisor>>8, 0x40);
}

// accuracy can be off by a lot (1 ms error plus any if it misses a count)
void timer_sleep(unsigned int ms) {
  unsigned int ms_past = 0;
  unsigned int is_upper = 0;

  while (ms_past < ms) {
    uint16_t pit_value = inb(0x40) + (((uint16_t)inb(0x40)) << 8);

    if (is_upper != (pit_value > pit_divisor/2)) {
      is_upper = !is_upper;
      if (is_upper) {
        ms_past += 2; // TODO: should be 1, but setting to 2 made timing more accurate without interrupts
      }
    }
  }
}