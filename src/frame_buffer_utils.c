#include <stddef.h>
#include <stdint.h>
#include "frame_buffer_utils.h"
#include "bitmap.h"

static uint32_t pack_color(Color color, FrameBuffer *fb) {
	// cast everything to 32 bit ints
	uint32_t red = (uint32_t)color.r;
	uint32_t green = (uint32_t)color.g;
	uint32_t blue = (uint32_t)color.b;
	uint32_t alpha = (uint32_t)color.a;
	
	// shift each byte to its respective in the final 32 bit color
	return (red << fb->red_shift |
		green << fb->green_shift |
		blue << fb->blue_shift |
		alpha << fb->alpha_shift);
}

void put_pixel(FrameBuffer *fb, Point point, Color color) {
	// ignore any pixels that are not on the screen
	if ((point.x < 0 || (uint32_t)point.x >= fb->width) || (point.y < 0 || (uint32_t)point.y >= fb->height)) {
		return;
	}

	// calculate the offset for this pixel
	size_t offset = point.y * fb->pitch + point.x * sizeof(uint32_t);
	uint32_t *pixel = (uint32_t*)(fb->buffer + offset);
	*pixel = pack_color(color, fb);
}

void put_pixel_pre_packed(FrameBuffer *fb, Point point, uint32_t color) {
	// ignore any pixels that are not on the screen
	if ((point.x < 0 || (uint32_t)point.x >= fb->width) || (point.y < 0 || (uint32_t)point.y >= fb->height)) {
		return;
	}

	// calculate the offset for this pixel
	size_t offset = point.y * fb->pitch + point.x * sizeof(uint32_t);
	uint32_t *pixel = (uint32_t*)(fb->buffer + offset);
	*pixel = color; // this is just a small optimization so that we don't have to always repeatedly pack the same color
}

void clear_screen(FrameBuffer *fb, Color color) {
	uint32_t packed_color = pack_color(color, fb);

	// we don't need to call pack_color for every pixel since they're all the same
	for (uint32_t y = 0; y < fb->height; y++) {
		uint32_t *row_loc = (uint32_t*)fb->buffer + ((size_t)y * fb->pitch);
		for (uint32_t x = 0; x < fb->width; x++) {
			row_loc[x] = packed_color;
		}
	}
}

void draw_bitmap(FrameBuffer *fb, Bitmap *bitmap, Point loc, Color color, uint8_t scale) {
	if (!fb || !bitmap || !bitmap->data || scale <= 0) {
		return;
	}

	// TODO: Implement scale parameter (rn it just does 1x)
	uint32_t packed_color = pack_color(color, fb); // prepack since it's the same for all pixels
	for (uint32_t row = 0; row < bitmap->height; row++) {
		for (uint32_t col = 0; col < bitmap->width; col++) {
			uint32_t byte_index = row * bitmap->stride + (col / 8u); // calculate the index rq
			
			// this byte represents 8 pixels - 1 means we need to fill the pixel in with color
			for (int i = 0; i < 8 * bitmap->stride; i++) {
				uint8_t bit = (bitmap->data[byte_index] >> i) & 1; // get the bit value by shifting it to the lsb and masking with 1
				if (bit) {
					Point coord = {loc.x + i, loc.y + row}; // add the column to the starting x and row to y
					put_pixel_pre_packed(fb, coord, packed_color);
				}
			}
		}
	}	
}
