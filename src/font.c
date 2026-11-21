#include <stdint.h>
#include <stddef.h>

#include "frame_buffer_utils.h"
#include "bitmap.h"
#include "font.h"

void draw_text(FrameBuffer *fb, char *text, Point loc, Color color, uint8_t scale) {
	size_t original_x = loc.x;	
	for (char *p = text; *p != '\0'; p++) {
		char ch = *p;
		if (ch == '\n') {
			// move down
			loc.y += (FONT_HEIGHT + 2) * scale;
			loc.x = original_x; // set it back to be left aligned
			continue;
		}
		// we want to loop through eahc character of the text and render it (w/ padding)
		if (ch < FONT_FIRST || ch > FONT_LAST) {
			ch = '?';
		}

		Bitmap glyph = {.data = font[ch - FONT_FIRST], .width = FONT_WIDTH, .height = FONT_HEIGHT, .stride = 1};
		draw_bitmap(fb, &glyph, loc, color, scale);
		
		// update position by adding FONT_WIDTH and 2px padding
		loc.x += (FONT_WIDTH + 2) * scale;		
	}
}
