#ifndef _QUAD_H_
#define _QUAD_H_

#include "frame_buffer_utils.h"
#include "general_utils.h"

typedef struct {
	Point top;
	float bottom_y;
	float islope;
} ScanEdge;

/*
* A function to fill in a quadrilaterial defined by the 4 corner points
*
* @param pixels - A PixelList to add pixels to
* @param corners - A list of 4 Points representing the corners of the quadrilateral
* @param color - The Color of the quadrilateral
* @return int - 1 on success, 0 on failure
*/
int fill_quad(PixelList *pixels, Point corners[4], Color color, int antialiasing);

#endif
