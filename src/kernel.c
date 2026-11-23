#include "asm.h"
#include "descriptors.h"
#include "general_utils.h"
#include "multiboot.h"
#include "frame_buffer_utils.h"
#include "font.h"
#include "line.h"
#include "quad.h"
#include "timer.h"
#include "colors.h"
#include "circle.h"


FrameBuffer fb;

INTERRUPT_ENTRY(timer_int)
void timer_int() {
	draw_text(&fb, "Named Interrupt!!", (Point){200, 20}, WHITE, 1);
}

static gdt_t gdt[256];
static idt_t idt[256];


void kernel_main(unsigned long magic, multiboot_info_t *info) {
	set_idt(idt+0x80, timer_int_entry);
	init_descriptors(gdt, idt);

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
