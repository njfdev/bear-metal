#include "polygon.h"

int draw_polygon_center(PixelList *pixels, Point center, int num_sides, int circumradius, Color color, int thickness, int fill, int antialiasing) {
	// TODO: Fix thicker lines not being drawn correctly at vertices
	// we just need to calculate each point and then draw lines between them
	Point vertices[num_sides];
	float angle_mult = (4.0f * HALF_PI) / num_sides;
	for (int i = 1; i <= num_sides; i++) {
		float angle = angle_mult * i;
		int y = (int)(sin(angle) * circumradius + center.y);
		int x = (int)(sin(angle + HALF_PI) * circumradius + center.x);
		vertices[i - 1] = (Point){x, y};
	}	
	return draw_polygon_vertices(pixels, vertices, num_sides, color, thickness, fill, antialiasing);
}

int draw_polygon_vertices(PixelList *pixels, Point *vertices, int num_sides, Color color, int thickness, int fill, int antialiasing) {
	int pixel_count = pixels->count;

	for (int i = 0; i < num_sides; i++) {
		if (!draw_line(pixels, vertices[i], vertices [(i + 1) % num_sides], color, antialiasing, thickness)) {
			pixels->count = pixel_count;
			return 0;
		}
	}

	if (fill) {	
		ScanEdge edges[num_sides];
		int edge_count = 0;
		float top_y = vertices[0].y;
		float bottom_y = vertices[0].y;

		for (int i = 0; i < num_sides; i++) {
			// loop through pairs of points to construct edges
			Point a = vertices[i];
			Point b = vertices[(i + 1) % num_sides];

			if (a.y == b.y) {
				// we don't care abt this edge
				continue;
			}

			if (a.y > b.y) {
				Point temp = a;
				a = b;
				b = temp;
			}
			
			float dx = (float)b.x - (float)a.x;
			float dy = (float)b.y - (float)a.y;		
			edges[edge_count] = (ScanEdge){a, b.y, dx / dy};
			edge_count++;

			if (a.y < top_y) {
				top_y = a.y;
			}
			if (b.y > bottom_y) {
				bottom_y = b.y;
			}
		}

		for (int y = top_y; y < bottom_y; y++) {	
			// loop through each y value in the quadrilateral
			float left = 100000.0f; // arbitrarily large
			float right = 0.0f;
			for (int i = 0; i < edge_count; i++) { 
				ScanEdge *edge = &edges[i];
				if (y < edge->top.y || y >= edge->bottom_y) {
					continue;
				}

				float x = edge->top.x + (y - edge->top.y) * edge->islope;
				if (x < left) {
					left = x;
				}
				if (x > right) {
					right = x;
				}
			}
			
			for (int x = left; x < right; x++) {
				// we need to antialias only on the edges - we do this by finding distance from the true float value
				if (antialiasing && x == (int)left) {
					// x <= left so we just change opacity of this pixel
					int alpha = (int)(color.a * (1 - (left - x)));
					if (!add_pixel_to_array(pixels, (Point){x, y}, (Color){color.r, color.g, color.b, alpha})) {
						pixels->count = pixel_count;
						return 0;
					}
				} else if (antialiasing && x >= (right - 1)) {
					// we need to color this pixel and possibly the pixel after it
					if (!add_pixel_to_array(pixels, (Point){x, y}, color)) {
						pixels->count = pixel_count;
						return 0;
					}
					int alpha = (int)(color.a * (right - x));
					if (!add_pixel_to_array(pixels, (Point){x + 1, y}, (Color){color.r, color.g, color.b, alpha})) {
						pixels->count = pixel_count;
						return 0;
					}
				} else {
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
