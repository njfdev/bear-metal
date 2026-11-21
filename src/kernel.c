#include "multiboot.h"
#include "frame_buffer_utils.h"
#include "font.h"
#include "line.h"
#include "quad.h"

void kernel_main(unsigned long magic, multiboot_info_t *info) {
	FrameBuffer fb = {
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
	Color white = {255, 255, 255, 0};	
	Color black = {0, 0, 0, 0};
	Color red = {255, 0, 0, 0};
	Color green = {0, 255, 0, 0};
	Color blue = {0, 0, 255, 0};

	// set the background to be gray
	clear_screen(&fb, background);
	// draw some example text
	draw_text(&fb, "Hello World", (Point){20, 20}, white, 1);
	// draw a line
	draw_line(&fb, (Point){30, 260}, (Point){100, 220}, white, 0, 5); 
	// draw a quad
	Point corners[4] = {(Point){50, 50}, (Point){110, 50}, (Point){120, 140}, (Point){40, 130}};
	fill_quad(&fb, corners, red);
}
