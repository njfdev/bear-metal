#include "keyboard.h"
#include "asm.h"
#include "descriptors.h"

// bit order: 0, 0, 0, enter, right, left, down, up
static volatile uint8_t key_mask = 0;
static volatile uint8_t last_byte = 0;

INTERRUPT_ENTRY(keyboard_int)
void keyboard_int() {
  // read byte
  uint8_t byte = inb(0x60);

  if (byte != 0xE0 && last_byte == 0xE0) {
    switch (byte) {
      case 0x48: // cursor up pressed
        key_mask |= 0b00000001;
        break;
      case 0x4b: // cursor left pressed
        key_mask |= 0b00000100;
        break;
      case 0x4d: // cursor right pressed
        key_mask |= 0b00001000;
        break;
      case 0x50: // cursor down pressed
        key_mask |= 0b00000010;
        break;


      case 0xc8: // cursor up released
        key_mask &= 0b11111110;
        break;
      case 0xcb: // cursor left released
        key_mask &= 0b11111011;
        break;
      case 0xcd: // cursor right released
        key_mask &= 0b11110111;
        break;
      case 0xd0: // cursor down released
        key_mask &= 0b11111101;
        break;
    }
  } else if (last_byte != 0xE0 && byte != 0xE0) {
    switch (byte) {
      case 0x1c: // enter pressed
        key_mask |= 0b00010000;
        break;
      case 0x9c: // enter released
        key_mask &= 0b11101111;
        break;
    }
  }

  last_byte = byte;

	// end of interrupt
	outb(0x20, 0x20);
}

void keyboard_init(idt_t *idt) {
  cli();

  // setup interrupt 0x21 (IRQ 1)
  set_idt(idt+0x21, keyboard_int_entry);

  // disable PS/2 device 1 and 2
  outb(0xad, 0x64);
  outb(0xa7, 0x64);

  // from now on only device 1 is configured (keyboard)

  // flush buffer by reading from it
  inb(0x60);
  
  // read current controller byte
  // outb(0x20, 0x64);
  // uint8_t controller_config_byte = inb(0x64);

  // update config byte to enable device 1
  uint8_t controller_config_byte = 0b01100101;
  outb(0x60, 0x64);
  outb(controller_config_byte, 0x64);

  // enable device 1
  outb(0xae, 0x64);

  sti();
}

uint8_t check_pressed(KeyCode code) {
  return (key_mask >> code) & 1;
}