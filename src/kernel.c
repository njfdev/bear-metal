#include "asm.h"
#include "descriptors.h"
#include "gameloop.h"
#include "general_utils.h"
#include "keyboard.h"
#include "multiboot.h"
#include "frame_buffer_utils.h"
#include "font.h"
#include "line.h"
#include "quad.h"
#include "timer.h"
#include "colors.h"
#include "circle.h"
#include "polygon.h"

FrameBuffer fb = {};

static gdt_t gdt[256];
static idt_t idt[256];


void kernel_main(unsigned long magic, multiboot_info_t *info) {
	init_descriptors(gdt, idt);
	timer_init(idt);
	keyboard_init(idt);

  // unmask all IRQ 0 and 1 of PIC
  outb(0b11111100, 0x21);

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

	static Pixel line_store[16384];
	PixelList pixels = {.pixels = line_store, .capacity = sizeof(line_store) / sizeof(line_store[0]), .count = 0};
	gameloop(&fb, &pixels);

	Color background = {20, 20, 20, 0};

	// set the background to be gray
	clear_screen(&fb, background);
	// draw_polygon_center(&pixels, (Point){200, 200}, 4, 80, SHAMROCK, 1, 0, 0);
	// draw some example text
	// draw_text(&pixels, "Hello World", (Point){20, 20}, WHITE, 1);
	// draw a line
	// draw_line(&pixels, (Point){200, 260}, (Point){300, 220}, OCRE, 0, 5); 
	// draw a quad
	// Point corners[4] = {(Point){50, 50}, (Point){110, 50}, (Point){120, 140}, (Point){40, 130}};
	// fill_quad(&pixels, corners, RED, 0);
	// draw a circle
	// draw_circle(&pixels, (Point){250, 250}, 50, BURPLE, 5, 0);

	int count = 0;

	int i = 0;
	while (1) {
		draw_pixel_array(&fb, &pixels);
	}
}
