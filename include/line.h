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
