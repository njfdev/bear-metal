#include <stdint.h>
#include <stddef.h>
#include "quad.h"

void fill_quad(FrameBuffer *fb, Point corners[4], Color color) {	
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
		float left = 100000.0f; // arbitrarily large
		float right = 0.0f;
		// loop through each y value in the quadrilateral
		for (int i = 0; i < edge_count; i++) { 
			ScanEdge *edge = &edges[i];
			if (y < edge->top.y || y >= edge->bottom_y) {
				continue;
			}

			float x = edge->top.x + (y - edge->top.y) * edge->islope;
			if ( x < left) {
				left = x;
			}
			if (x > right) {
				right = x;
			}
		}

		for (int x = left; x < right; x++) {
			put_pixel_pre_packed(fb, (Point){x, y}, packed_color);
		}
	}
}
