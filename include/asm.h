#ifndef _ASM_H_
#define _ASM_H_

#include <stdint.h>

static inline void outb(uint8_t value, uint16_t port) {
  asm volatile ("outb %0, %1"
    :
    : "a" (value), "Nd" (port)
    : "memory"  // prevent memory compiler clobbering
  );
}

static inline uint8_t inb(uint16_t port) {
  uint8_t val;
  asm volatile ("inb %1, %0"
    : "=a" (val)
    : "Nd" (port)
    : "memory"  // prevent memory compiler clobbering
  );
  return val;
}

static inline void cli() {
  asm volatile ("cli");
}

static inline void sti() {
  asm volatile ("sti");
}

#endif