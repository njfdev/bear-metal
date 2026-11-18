#ifndef _BITMAP_H_
#define _BITMAP_H_

#include <stdint.h>

typedef struct {
	uint8_t *data;
	uint16_t width;
	uint16_t height;
	uint16_t stride; // number of bytes in each row of the bitmap
} Bitmap;

#endif
