#include "circle.h"

static void plot_octants(FrameBuffer *fb, Point center, Point mirror_point, uint32_t color) {
	Point mirrored_points[8];
	mirrored_points[0] = (Point){center.x + mirror_point.x, center.y + mirror_point.y};
	mirrored_points[1] = (Point){center.x + mirror_point.x, center.y - mirror_point.y};
	mirrored_points[2] = (Point){center.x - mirror_point.x, center.y + mirror_point.y};
	mirrored_points[3] = (Point){center.x - mirror_point.x, center.y - mirror_point.y};
	mirrored_points[4] = (Point){center.x + mirror_point.y, center.y + mirror_point.x};
	mirrored_points[5] = (Point){center.x + mirror_point.y, center.y - mirror_point.x};
	mirrored_points[6] = (Point){center.x - mirror_point.y, center.y + mirror_point.x};
	mirrored_points[7] = (Point){center.x - mirror_point.y, center.y - mirror_point.x};
	for (int i = 0; i < 8; i++) {
		put_pixel_pre_packed(fb, mirrored_points[i], color);
	}
}

void draw_circle(FrameBuffer *fb, Point center, int radius, Color color, int thickness, int fill) {
	// midpoint circle algorithm with two passes and then a scanline fill if needed
	uint32_t packed_color = pack_color(color, fb);
	int x = 0;
	// if we are filling and we have a thickness we don't need to draw inner circle
	int y = (thickness > 1 && fill) ? (radius + thickness) : radius;
	int err = 1 - y;

	while (x <= y) {
		if (fill) {
			for (int y_temp = x; y_temp <= y; y_temp++) {
				plot_octants(fb, center, (Point){x, y_temp}, packed_color);
			}
		} else {
			plot_octants(fb, center, (Point){x, y}, packed_color);
		}

		x++;
		if (err < 0) {
			err += 2 * x + 1;
		} else {
			y--;
			err += 2 * (x - y) + 1;
		}
	}
	
	if (thickness > 1 && fill) {
		return;
	}

	if (thickness > 1) {
		// then just plot another circle with radius + thickness and fill
		// it would be great if we could just recursively call, but sadly we have to do both circles at the same time so we can fill them in
		int x1 = 0;
		int y1 = radius;
		int x2 = 0;
		int y2 = radius + thickness;
		int err1 = 1 - radius;
		int err2 = 1 - (radius + thickness);
		for (int y = y1; y <= y2; y++) {
			plot_octants(fb, center, (Point){0, y}, packed_color);
		}

		while (x2 < y2) {
			x2++;
			if (x1 < y1) {
				x1++;
				if (err1 < 0) {
					err1 += 2 * x1 + 1;
				} else {
					y1--;
					err1 += 2 * (x1 - y1) + 1;
				}
			}
			
			if (err2 < 0) {
				err2 += 2 * x2 + 1;
			} else {
				y2--;
				err2 += 2 * (x2 - y2) + 1;
			}
			
			// now we need to fill it in
			int start_y = x2;
			if (x1 == x2 && y1 >= x2) {
				start_y = y1;
			}
			for (int y = start_y; y <= y2; y++) {
				plot_octants(fb, center, (Point){x2, y}, packed_color);
			}
		}
	}
}
