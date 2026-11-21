#include <stddef.h>
#include <stdint.h>
#include "bitmap.h"

void draw_bitmap(FrameBuffer *fb, Bitmap *bitmap, Point loc, Color color, uint8_t scale) {
	if (!fb || !bitmap || !bitmap->data || scale <= 0) {
		return;
	}

	uint32_t packed_color = pack_color(color, fb); // prepack since it's the same for all pixels
	for (uint32_t row = 0; row < bitmap->height; row++) {
		for (uint32_t col = 0; col < bitmap->width; col++) {
			uint32_t byte_index = row * bitmap->stride + (col / 8u); // calculate the index rq
			uint8_t mask = (uint8_t)(0x80u >> (col % 8u)); // create a mask to get the bit for the current pixel
			if ((bitmap->data[byte_index] & mask) == 0) {
				continue; // skip if we don't draw anything on this pixel
			}

			uint64_t start_x = (uint64_t)(loc.x + scale * col);
			uint64_t start_y = (uint64_t)(loc.y + scale * row);
			
			// each pixel becomes a scale by scale square
			for (uint32_t dy = 0; dy < scale; dy++) {
				for (uint32_t dx = 0; dx < scale; dx++) {
					uint64_t x = start_x + dx;
					uint64_t y = start_y + dy;
					
					put_pixel_pre_packed(fb, (Point){x, y}, packed_color);
				}
			}
		}
	}	
}
