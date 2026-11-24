#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "descriptors.h"

typedef enum {
  Up = 0,
  Down = 1,
  Left = 2,
  Right = 3,
  Enter = 4
} KeyCode;

void keyboard_init(idt_t *idt);
uint8_t check_pressed(KeyCode code);

#endif