#include <stdint.h>
#include <stddef.h>
#include "frame_buffer_utils.h"
#include "line.h"
#include "general_utils.h"

void draw_line(FrameBuffer *fb, Point start, Point end, Color color, int antialiasing, int thickness) {
	// TODO: Implement antialiasing flag and thickness
	int alpha = color.a;

	int dy = end.y - start.y;
	int dx = end.x - start.x;
	float m;

	// calculate the slope avoiding dividing by 0
	if (!dx) {
		m = 1;
	} else {
		m = (float)dy / dx;
	}	
	for (int i = 0; i < dx + 1; i++) {
		// loop through each x value 
		int x = start.x + i;
		float y = start.y + m * i;
		
		// now we need to find the opacity of the pixel above and below the line (int)y and (int)y+1
		// opacity of (int)y+1 is distance between line and (int)y and vice versa
		float distance = decimal_part(y);
		float top_opacity = 1 - distance;

		Color top_pixel = {color.r, color.g, color.b, color.a * top_opacity};
		Color bottom_pixel = {color.r, color.g, color.b, color.a * distance};
		put_pixel(fb, (Point){x, (int)y}, top_pixel);
		put_pixel(fb, (Point){x, (int)y + 1}, bottom_pixel);
	}
	
}
