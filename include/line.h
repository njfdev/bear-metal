#ifndef _LINE_H_
#define _LINE_H_

#include "frame_buffer_utils.h" 

/*
* A function to draw a 1px wide line with no antialiasing
*
* @param fb - A pointer to the frame buffer to draw on
* @param start - A Point representing the starting point of the line
* @param end - A Point representing the end point of the line
* @param color - The Color to draw the line
*/
static void draw_line_no_thickness(FrameBuffer *fb, Point start, Point end, Color color);

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
* @param fb - A pointer to the frame buffer
* @param start - The start Point of the line
* @param end - The end Point of the line
* @param color - The Color of the line
*/
static void draw_line_xiaolin(FrameBuffer *fb, Point start, Point end, Color color);

/*
* A function to draw a line on the provided frame buffer
*
* @param fb - A pointer to the frame buffer to draw on
* @param start - A Point representing the starting point of the line
* @param end - A Point representing the end point of the line
* @param color - The Color to draw the line
* @param antialiasing - Whether or not to antialias the line
* @param thickness - The thickness of the line (in px)
*/
void draw_line(FrameBuffer *fb, Point start, Point end, Color color, int antialiasing, int thickness);

#endif
