#include <stdint.h>
#include <stddef.h>

#include "frame_buffer_utils.h"
#include "bitmap.h"
#include "font.h"

void draw_text(FrameBuffer *fb, char *text, Point loc, Color color, uint8_t scale) {
	for (char *p = text; *p != '\0'; p++) {
		if (*p == '\n') {
			// move down
			loc.y += FONT_HEIGHT + 2;
			continue;
		}
		// we want to loop through eahc character of the text and render it (w/ padding)
		if (*p < FONT_FIRST || *p > FONT_LAST) {
			*p = '?';
		}

		Bitmap glyph = {.data = font[*p - FONT_FIRST], .width = FONT_WIDTH, .height = FONT_HEIGHT, .stride = 1};
		draw_bitmap(fb, &glyph, loc, color, scale);
		
		// update position by adding FONT_WIDTH and 2px padding
		loc.x += FONT_WIDTH + 2;		
	}
}
