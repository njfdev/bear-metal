#ifndef _WRITE_FRAME_BUFFER_H_
#define _WRITE_FRAME_BUFFER_H_

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
} Color;

typedef struct {
	// address 
	uint8_t *buffer;
	
	// size of screen
	uint32_t width;
	uint32_t height;
	uint32_t pitch; // width plus padding
	
	// location of colors
	uint8_t red_shift;
	uint8_t green_shift;
	uint8_t blue_shift;
	uint8_t alpha_shift;
} FrameBuffer;

static uint32_t pack_rgb(Color color, FrameBuffer *fb);
void put_pixel(FrameBuffer *fb, int x, int y, Color color); 

#endif
