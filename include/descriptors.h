#ifndef _DESCRIPTORS_H_
#define _DESCRIPTORS_H_

#include <stdint.h>

// macro to create a <name>_entry into the macro
#define INTERRUPT_ENTRY(name) \
__attribute__((naked)) void name##_entry() { \
  __asm__ volatile ("call " #name); \
  __asm__ volatile ("iret"); \
}

typedef struct {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) segment_info_t;

typedef struct {
	uint16_t limit_lower;
	uint16_t base_lower;
	uint8_t base_middle;
	uint8_t access;
	uint8_t limit_upper_and_flags;
	uint8_t base_upper;
} __attribute__((packed)) gdt_t;

typedef struct {
	uint16_t offset_low;
	uint16_t selector;
	uint8_t unused;
	uint8_t attributes;
	uint16_t offset_high;
} __attribute__((packed)) idt_t;


/* 
 * Prepares the Global and Interrupt Descriptor Tables with
 * the necessary descriptors for operation (text and data
 * GDT descriptors). Also loads the corresponding registers
 * for the CPU to locate the tables.
 * 
 * @param gdt Location of static 256 element long gdt_t array
 * @param idt Location of static 256 element long idt_t array
*/
void init_descriptors(gdt_t gdt[256], idt_t idt[256]);

/*
 * Sets a GDT entry applying to the entire ram with a certain
 * access configuration.
 * 
 * @param gdt Pointer to the GDT entry to set
 * @param access Access byte as described by https://wiki.osdev.org/Global_Descriptor_Table#Segment_Descriptor
*/
void set_gdt(gdt_t *gdt, uint8_t access);

/*
 * Sets an IDT entry as an interrupt gate type (0x8e) pointing
 * to a given interrupt handler function.
 * 
 * @param idt Pointer to the IDT entry to set
 * @param handler Interrupt handler function
*/
void set_idt(idt_t *idt, void handler(void));

#endif