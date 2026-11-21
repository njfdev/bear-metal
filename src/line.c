#include <stdint.h>
#include <stddef.h>
#include "frame_buffer_utils.h"
#include "line.h"
#include "general_utils.h"

void draw_line(FrameBuffer *fb, Point start, Point end, Color color, int antialiasing, int thickness) {
	// TODO: Implement thickness
	int alpha = color.a;

	int dy = end.y - start.y;
	int dx = end.x - start.x;
	if (antialiasing) {
		// we are using xiaolin
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
	} else {
		// in this case we want to use bresenham's
		uint32_t packed_color = pack_color(color, fb);
		
		int x = (int)start.x;
		int y = (int)start.y;
		int dx = (int)(end.x - start.x);
		int dy = (int)(end.y - start.y);
		int step_x = dx < 0 ? -1 : 1;
		int step_y = dy < 0 ? -1 : 1;
		dx = abs(dx);
		dy = abs(dy);
		int error = dx - dy;

		while (1) {
			put_pixel_pre_packed(fb, (Point){x, y}, packed_color);
			
			if (x == end.x && y == end.y) {
				break;
			}
			
			int double_err = 2 * error;
			
			if (double_err >= -dy) {
				// this means we move in x
				x += step_x;	
				error -= dy;
			}
			if (double_err <= dx) {
				y += step_y;
				error += dx;
			}
		}
	}	
}
