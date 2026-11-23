#include "asm.h"
#include "general_utils.h"
#include "multiboot.h"
#include "frame_buffer_utils.h"
#include "font.h"
#include "line.h"
#include "quad.h"
#include "timer.h"
#include "colors.h"
#include "circle.h"

struct Gdt {
	uint16_t limit_lower;
	uint16_t base_lower;
	uint8_t base_middle;
	uint8_t access;
	uint8_t limit_upper_and_flags;
	uint8_t base_upper;
} __attribute__((packed));

struct TableDescriptor {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

struct Gdt gdt[256];
struct TableDescriptor gdt_descriptor = {
	.limit = sizeof(gdt),
	.base = (uint32_t) &gdt
};

struct Idt {
	uint16_t offset_low;
	uint16_t selector;
	uint8_t unused;
	uint8_t attributes;
	uint16_t offset_high;
} __attribute__((packed));

struct Idt idt[256];
struct TableDescriptor idt_descriptor = {
	.limit = sizeof(idt),
	.base = (uint32_t) &idt
};

FrameBuffer fb;

void interrupt_handler() {
	draw_text(&fb, "Interrupted!!", (Point){200, 20}, WHITE, 1);
}
extern void interrupt_handler_entry(void);


void kernel_main(unsigned long magic, multiboot_info_t *info) {
	// setup GDT for code segment
	gdt[2] = (struct Gdt) {
		.limit_lower = 0xffff,
		.base_lower = 0,
		.base_middle = 0,
		.access = 0b10011010,
		.limit_upper_and_flags = (0xc0) + 0xf,
		.base_upper = 0
	};

	// setup GDT for data segment
	gdt[1] = (struct Gdt) {
		.limit_lower = 0xffff,
		.base_lower = 0,
		.base_middle = 0,
		.access = 0b10010010,
		.limit_upper_and_flags = (0xc0) + 0xf,
		.base_upper = 0
	};

	idt[0x80] = (struct Idt) {
		.offset_low = (uint32_t)&interrupt_handler_entry & 0xffff,
		.selector = 0x10,
		.attributes = 0x8e,
		.offset_high = (uint32_t)&interrupt_handler_entry >> 16,
	};

  // load descriptor tables
  __asm__ volatile ("lgdt %0"
		:
		: "m" (gdt_descriptor)
		:
	);
  __asm__ volatile ("lidt %0"
		:
		: "m" (idt_descriptor)
		:
	);

	// TODO: pull correct video dimensions from multiboot info
	fb = (FrameBuffer) {
		.buffer = (uint32_t*)0x80000000,
		.width = 640,
		.height = 480,
		.pitch = 640,
		.red_shift = 16,
		.green_shift = 8,
		.blue_shift = 0,
		.alpha_shift = 24
	};

	Color background = {20, 20, 20, 0};

	// set the background to be gray
	clear_screen(&fb, background);
	// draw some example text
	draw_text(&fb, "Hello World", (Point){20, 20}, WHITE, 1);
	// draw a line
	draw_line(&fb, (Point){200, 260}, (Point){300, 220}, OCRE, 0, 5); 
	// draw a quad
	Point corners[4] = {(Point){50, 50}, (Point){110, 50}, (Point){120, 140}, (Point){40, 130}};
	fill_quad(&fb, corners, RED, 0);
	// draw a circle
	draw_circle(&fb, (Point){250, 250}, 50, BURPLE, 5, 0);


	__asm__ volatile("int $0x80");

	int count = 0;

	int i = 0;
	while (1) {
		char int_str[20] = "Seconds: ";
		uint_to_string(count, int_str+9, 11);

		i++;

		if (i == 20) {
			i = 0;
			clear_screen(&fb, background);
		}

		draw_text(&fb, int_str, (Point){20, 40 + 20*i}, WHITE, 1);
		
		timer_sleep(1000);
		count += 1;
	}
}
