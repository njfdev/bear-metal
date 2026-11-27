#include <stdint.h>
#include <stddef.h>
#include "frame_buffer_utils.h"
#include "line.h"
#include "general_utils.h"
#include "quad.h"

static int draw_line_bresenham(PixelList *pixels, Point start, Point end, Color color) {
	int x = (int)start.x;
	int y = (int)start.y;
	int dx = (int)(end.x - start.x);
	int dy = (int)(end.y - start.y);
	int step_x = dx < 0 ? -1 : 1;
	int step_y = dy < 0 ? -1 : 1;
	dx = abs(dx);
	dy = abs(dy);
	int error = dx - dy;
	
	int pixel_count = pixels->count;
	while (1) {
		if (!add_pixel_to_array(pixels, (Point){x, y}, color)) {
			pixels->count = pixel_count;
			return 0;
		}

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
	return 1;
}

static void find_quad_corners(Point start, Point end, int thickness, Point corners[4]) {	
	int dx = end.x - start.x;
	int dy = end.y - start.y;

	// in this case, we rly want to draw a filled in rect
	// the math is really annoying here, and we don't have a square root function - we can use quake III inverse square root video thingy					      // we want 1/sqrt(m^2 + 1) which is equivalent to dx/sqrt(dx^2 + dy^2) so no float division
	float val = dx * dx + dy * dy;
	float isqrt = inverse_sqrt(val);
	float c = abs(dx) * isqrt;
	float mc = sgn(dx) * dy * isqrt;
	float d = thickness / 2.0f;
	
	// now we just create the points
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

	corners[0] = (Point){x1, y1};
	corners[1] = (Point){x2, y2};
	corners[2] = (Point){x4, y4};
	corners[3] = (Point){x3, y3};
}

static int draw_line_xiaolin(PixelList *pixels, Point start, Point end, Color color) {
	// TODO: Implement an alpha channel
	int x0 = start.x;
	int y0 = start.y;
	int x1 = end.x;
	int y1 = end.y;
	int steep = abs(y1 - y0) > abs(x1 - x0);
	
	int pixel_count = pixels->count;

	if (steep) {
		// swap so we can step along x-axis still
		int temp = x0;
		x0 = y0;
		y0 = temp;

		temp = x1;
		x1 = y1;
		y1 = temp;
	}
	if (x0 > x1) {
		// draw from left to right
		int temp = x0;
		x0 = x1;
		x1 = temp;
		
		temp = y0;
		y0 = y1;
		y1 = temp;
	}
	int dx = x1 - x0;
	float slope = dx ? (float)(y1 - y0) / dx : 0.0f;
	for (int x = x0; x < x1; x++) {
		float exact_y;
		if (x == x1) {
			exact_y = y1;
		} else {
			exact_y = y0 + (x - x0) * slope;
		}
		int y = (int)exact_y;
		if (exact_y < y) {
			y--;
		}
		float fraction = exact_y - y;

		// find the opacities
		Color first = color;
		Color second = color;
		second.a = (uint8_t)(color.a * fraction + 0.5f); // round
		first.a = color.a - second.a; // opacities will sum to original
		
		if (first.a > 0) {
			Point pixel = steep ? (Point){y, x} : (Point){x, y}; // swap back if needed
			if (!add_pixel_to_array(pixels, pixel, color)) {
				pixels->count = pixel_count;
				return 0;
			}
		}
		if (second.a > 0) {
			Point pixel = steep ? (Point){y, x} : (Point){x, y};	
			if (!add_pixel_to_array(pixels, pixel, color)) {
				pixels->count = pixel_count;
				return 0;
			}
		}
	}
	return 1;
}

int draw_line(PixelList *pixels, Point start, Point end, Color color, int antialiasing, int thickness) {
	if (antialiasing) {
		// we are using xiaolin
		if (thickness > 1) {
			Point corners[4]; 
			find_quad_corners(start, end, thickness, corners);
			return fill_quad(pixels, corners, color, 1);
		} else {
			return draw_line_xiaolin(pixels, start, end, color);
		}
	} else {
		// in this case we want to use bresenham's
		if (thickness > 1) {
			Point corners[4]; 
			find_quad_corners(start, end, thickness, corners);
			return fill_quad(pixels, corners, color, 0);
		} else {
			return draw_line_bresenham(pixels, start, end, color);
		}
	}
}
