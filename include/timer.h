#ifndef _TIMER_H_
#define _TIMER_H_

#include "descriptors.h"

void timer_init(idt_t *idt);
void timer_sleep(unsigned int ms);
uint32_t millis();

#endif