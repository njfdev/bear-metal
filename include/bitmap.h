#ifndef _BITMAP_H_
#define _BITMAP_H_

#include <stdint.h>
#include "frame_buffer_utils.h"

typedef struct {
	const uint8_t *data;
	uint16_t width;
	uint16_t height;
	uint16_t stride; // number of bytes in each row of the bitmap
} Bitmap;

/*
* A function to draw a bitmap with the upper left corner at the specified point
*
* @param bitmap - A pointer to the bitmap to be drawn
* @param loc - A Point representing where the upper left corner of the bitmap will be drawn
* @param color - A Color representing the color of the bitmap
* @param scale - The scale of the bitmap
* @return int - 1 on success, 0 on failure
*/
void draw_bitmap(Bitmap *bitmap, Point loc, Color color, uint8_t scale);

#endif
