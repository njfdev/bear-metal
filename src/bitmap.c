#include "bitmap.h"

int draw_bitmap(PixelList *pixels, Bitmap *bitmap, Point loc, Color color, uint8_t scale) {
	int pixel_count = pixels->count;

	for (uint32_t row = 0; row < bitmap->height; row++) {
		for (uint32_t col = 0; col < bitmap->width; col++) {
			uint32_t byte_index = row * bitmap->stride + (col / 8u); // calculate the index rq
			uint8_t mask = (uint8_t)(0x80u >> (col % 8u)); // create a mask to get the bit for the current pixel
			if ((bitmap->data[byte_index] & mask) == 0) {
				continue; // skip if we don't draw anything on this pixel
			}

			int64_t start_x = (int64_t)(loc.x + scale * col);
			int64_t start_y = (int64_t)(loc.y + scale * row);
			
			// each pixel becomes a scale by scale square
			for (uint32_t dy = 0; dy < scale; dy++) {
				for (uint32_t dx = 0; dx < scale; dx++) {
					int64_t x = start_x + dx;
					int64_t y = start_y + dy;
					
					if (!add_pixel_to_array(pixels, (Point){x, y}, color)) {
						pixels->count = pixel_count;
						return 0;
					}
				}
			}
		}
	}	
	return 1;
}
