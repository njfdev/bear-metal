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
	// TODO: Implement thickness and fill
	// midpoint circle algorithm with two passes and then a scanline fill if needed
	uint32_t packed_color = pack_color(color, fb);
	int x = 0;
	int y = radius;
	int err = 1 - radius;
	
	plot_octants(fb, center, (Point){x, y}, packed_color);

	while (x < y) {
		x++;
		if (err < 0) {
			err += 2 * x + 1;
		} else {
			y--;
			err += 2 * (x - y) + 1;
		}
		plot_octants(fb, center, (Point){x, y}, packed_color);
	}
}
