#ifndef _CIRCLE_H_
#define _CIRCLE_H_

#include <stddef.h>
#include <stdint.h>
#include "frame_buffer_utils.h"

/*
* A function to mirror a point to all 8 octants for a circle
*
* @param fb - A pointer to the frame buffer
* @param center - The center Point of the circle
* @param mirror_point - The Point to mirror to all 8 octants
* @param color - The pre-packed color
*/
static void plot_octants(FrameBuffer *fb, Point center, Point mirror_point, uint32_t color);

/*
* A function to draw a circle
*
* @param fb - A pointer to the frame buffer
* @param center - The center Point of the circle
* @param radius - The radius of the circle (in px)
* @param color - The Color of the circle
* @param thickness - The stroke width (in px)
* @param fill - Whether or not to fill the circle (0 for no, 1 for yes)
*/
void draw_circle(FrameBuffer *fb, Point center, int radius, Color color, int thickness, int fill);

#endif
