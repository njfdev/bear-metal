#include <stdint.h>
#include <stddef.h>
#include "frame_buffer_utils.h"
#include "line.h"
#include "general_utils.h"
#include "quad.h"

static void draw_line_no_thickness(FrameBuffer *fb, Point start, Point end, Color color) {
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

void draw_line(FrameBuffer *fb, Point start, Point end, Color color, int antialiasing, int thickness) {
	// TODO: Implement thickness for antialiased
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
		if (thickness > 1) {
			int dx = end.x - start.x;
			int dy = end.y - start.y;

			// in this case, we rly want to draw a filled in rect
			// the math is really annoying here, and we don't have a square root function - we can use quake III inverse square root video thingy					      // we want 1/sqrt(m^2 + 1) which is equivalent to dx/sqrt(dx^2 + dy^2) so no float division
			float val = dx * dx + dy * dy;
			float isqrt = inverse_sqrt(val);
			float c = abs(dx) * isqrt;
			float mc = sgn(dx) * dy * isqrt;
			float d = thickness / 2.0f;
			
			// now we rly just want to draw two lines and fill in the pixels in between
			int x1, x2, x3, x4;
			int y1, y2, y3, y4;
			if (dx == 0) {
				// line 1
				x1 = round(start.x + d);
				y1 = start.y;
				x2 = round(end.x + d);
				y2 = end.y;

				// line 2
				x3 = round(start.x - d);
				y3 = start.y;
				x4 = round(end.x - d);
				y4 = end.y;
			} else {
				// line 1
				x1 = round(start.x + d * mc);
				y1 = round(start.y - d * c);
				x2 = round(end.x + d * mc);
				y2 = round(end.y - d * c);
				
				// line 2
				x3 = round(start.x - d * mc);
				y3 = round(start.y + d * c);
				x4 = round(end.x - d * mc);
				y4 = round(end.y + d * c);
			}

			Point corners[4] = {(Point){x1, y1}, (Point){x2, y2}, (Point){x4, y4}, (Point){x3, y3}};
			fill_quad(fb, corners, color);
		} else {
			draw_line_no_thickness(fb, start, end, color);
		}
	}
}
