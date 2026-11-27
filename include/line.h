#ifndef _LINE_H_
#define _LINE_H_

#include <stdint.h>
#include <stddef.h>
#include "frame_buffer_utils.h"
#include "general_utils.h"
#include "quad.h"

/*
* A function to draw a 1px wide line with no antialiasing
*
* @param pixels - A PixelList to keep track of pixels that need to be drawn
* @param start - A Point representing the starting point of the line
* @param end - A Point representing the end point of the line
* @param color - The Color to draw the line
* @param int - 1 on success, 0 on failure
*/
static int draw_line_bresenham(PixelList *pixels, Point start, Point end, Color color);

/*
* A function to find the four corners of a quadrilateral representing a line with thickness > 1
*
* @param start - The start Point of the line
* @param end - The end Point of the line
* @param thickness - The thickness of the line (in px)
* @return void - Modifies the list in place to create the 4 Points
*/
static void find_quad_corners(Point start, Point end, int thickness, Point corners[4]);

/*
* A helper function to draw a 1 thickness antialiased line
*
* @param pixels - A PixelList to keep track of pixels that need to be drawn
* @param start - The start Point of the line
* @param end - The end Point of the line
* @param color - The Color of the line
* @return int - 1 on success, 0 on failure
*/
static int draw_line_xiaolin(PixelList *pixels, Point start, Point end, Color color);

/*
* A function to draw a line on the provided frame buffer
*
* @param pixels - A PixelList to keep track of pixels that need to be drawn
* @param start - A Point representing the starting point of the line
* @param end - A Point representing the end point of the line
* @param color - The Color to draw the line
* @param antialiasing - Whether or not to antialias the line
* @param thickness - The thickness of the line (in px)
* @return int - 1 on success, 0 on failure
*/
int draw_line(PixelList *pixels, Point start, Point end, Color color, int antialiasing, int thickness);

#endif
