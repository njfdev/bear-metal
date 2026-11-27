#include "font.h"

int draw_text(PixelList *pixels, char *text, Point loc, Color color, uint8_t scale) {
	int pixel_count = pixels->count;
	
	int32_t original_x = loc.x;	
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
		if (!draw_bitmap(pixels, &glyph, loc, color, scale)) {
			pixels->count = pixel_count;
			return 0;
		}
		
		// update position by adding FONT_WIDTH and 2px padding
		loc.x += (FONT_WIDTH + 2) * scale;		
	}
	return 1;
}
