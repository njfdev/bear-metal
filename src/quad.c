#include <stdint.h>
#include <stddef.h>
#include "quad.h"

void fill_quad(FrameBuffer *fb, Point corners[4], Color color, int antialiasing) {	
	uint32_t packed_color = pack_color(color, fb);

	ScanEdge edges[4];
	int edge_count = 0;
	float top_y = corners[0].y;
	float bottom_y = corners[0].y;

	for (int i = 0; i < 4; i++) {
		// loop through pairs of points to construct edges
		Point a = corners[i];
		Point b = corners[(i + 1) % 4];

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
				put_pixel(fb, (Point){x, y}, (Color){color.r, color.g, color.b, alpha});
			} else if (antialiasing && x >= (right - 1)) {
				// we need to color this pixel and possibly the pixel after it
				put_pixel_pre_packed(fb, (Point){x, y}, packed_color);
				int alpha = (int)(color.a * (right - x));
				put_pixel(fb, (Point){x + 1, y}, (Color){color.r, color.g, color.b, alpha});
			} else {
				put_pixel_pre_packed(fb, (Point){x, y}, packed_color);
			}
		}
	}
}
