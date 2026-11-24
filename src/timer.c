#include "asm.h"
#include "descriptors.h"
#include <timer.h>

const unsigned int pit_divisor = 1193182/1000; // triggers at 1000hz

static volatile uint32_t total_millis = 0;

INTERRUPT_ENTRY(timer_int)
void timer_int() {
	total_millis+=1;
	// draw_text(&fb, "Named Interrupt!!", (Point){200, 20*count}, WHITE, 1);
	
	// end of interrupt
	outb(0x20, 0x20);
}

// TODO: set this up with interrupts
void timer_init(idt_t *idt) {
  // setup interrupt 0x20
  set_idt(idt+0x20, timer_int_entry);

  cli();

  // init PIC one
  outb(0x11, 0x20);
  outb(0x20, 0x21); // set master PIC offset to 0x20
  outb(0, 0x21);
  outb(1, 0x21); // use 8086 mode

	// setup 1000 hz timer on channel 0 of PIT
	outb(0x06, 0x43);
	outb(pit_divisor&0xff, 0x40);
	outb(pit_divisor>>8, 0x40);

  // unmask all IRQs interrupt of PIC
  outb(0xfe, 0x21);

  sti();
}

// accuracy can be off by a lot (1 ms error plus any if it misses a count)
void timer_sleep(unsigned int ms) {
  uint32_t ms_start = total_millis;

  while (ms > total_millis-ms_start) {
    __asm__ volatile ("hlt");
  }
}

uint32_t millis() {
  return total_millis;
}