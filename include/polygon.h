#ifndef _POLYGON_H_
#define _POLYGON_H_

#include <stddef.h>
#include <stdint.h>
#include "frame_buffer_utils.h"

/*
* A function to draw a regular polygon with an arbitrary number of sides
*
* @param fb - A pointer to the frame buffer
* @param center - The center Point of the polygon
* @param num_sides - The number of sides on the polygon (>=2)
* @param circumradius - The distance from the center point to a vertex
* @param color - The Color of the polygon
* @param thickness - The stroke width of the border of the polygon
* @param fill - Whether or not to fill the interior of the polygon (0 for no, 1 for yes)
* @param antialias - Whether or not to antialias (0 for no, 1 for yes)
*/
void draw_polygon_center(FrameBuffer *fb, Point center, int num_sides, int circumradius, Color color, int thickness, int fill, int antialiasing);

/*
* A function to draw a polygon with an arbitrary number of sides
*
* @param fb - A pointer to the frame buffer
* @param vertices - A list of Points defining the vertices
* @param num_sides - The number of sides on the polygon (>= 2)
* @param color - The Color of the polygon
* @param thickness - The stroke width of the border of the polygon
* @param fill - Whether or not to fill in the interior of the polygon (0 for no, 1 for yes)
* @param antialiasing - Whether or not to antialias (0 for no, 1 for yes)
*/
void draw_polygon_vertices(FrameBuffer *fb, Point *vertices, int num_sides, Color color, int thickness, int fill, int antialiasing);

#endif
