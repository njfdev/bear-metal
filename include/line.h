#ifndef _LINE_H_
#define _LINE_H_

#include "frame_buffer_utils.h"

void draw_line(FrameBuffer *fb, Point start, Point end, Color color, int antialiasing, int thickness);

#endif
