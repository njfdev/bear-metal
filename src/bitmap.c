#include <stddef.h>
#include <stdint.h>
#include "frame_buffer_utils.h"
#include "bitmap.h"

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
