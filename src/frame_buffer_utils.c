#include <stddef.h>
#include <stdint.h>
#include "frame_buffer_utils.h"

uint32_t pack_color(Color color, FrameBuffer *fb) {
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
	size_t offset = point.y * fb->pitch + point.x;
	volatile uint32_t *pixel = (volatile uint32_t*)(fb->buffer + offset);
	*pixel = pack_color(color, fb);
}

void put_pixel_pre_packed(FrameBuffer *fb, Point point, uint32_t color) {
	// ignore any pixels that are not on the screen
	if ((point.x < 0 || (uint32_t)point.x >= fb->width) || (point.y < 0 || (uint32_t)point.y >= fb->height)) {
		return;
	}

	// calculate the offset for this pixel
	size_t offset = point.y * fb->pitch + point.x;
	volatile uint32_t *pixel = (volatile uint32_t*)(fb->buffer + offset);
	*pixel = color; // this is just a small optimization so that we don't have to always repeatedly pack the same color
}

void clear_screen(FrameBuffer *fb, Color color) {
	uint32_t packed_color = pack_color(color, fb);

	uint32_t height = fb->height;
	uint32_t width = fb->width;

	// we don't need to call pack_color for every pixel since they're all the same
	for (uint32_t y = 0; y < height; y++) {
		volatile uint32_t *row_loc = (volatile uint32_t*)fb->buffer + ((size_t)y * fb->pitch);
		for (uint32_t x = 0; x < width; x++) {
			row_loc[x] = packed_color;
		}
	}

}

void draw_pixel_array(FrameBuffer *fb, PixelList pixels) {
	Color prev = pixels.pixels[0].color;
	uint32_t packed_prev = pack_color(prev, fb);
	for (int i = 0; i < pixels.count; i++) {
		Color color = pixels.pixels[i].color;
		// check if the color is the same -- start with alpha since most likely to be different
		if (color.a == prev.a && color.r == prev.r && color.g == prev.g && color.b == prev.b) {
			put_pixel_pre_packed(fb, pixels.pixels[i].pixel, packed_prev);
		} else {
			put_pixel(fb, pixels.pixels[i].pixel, color);
			prev = color;
			packed_prev = pack_color(prev, fb);
		}
	}
}
