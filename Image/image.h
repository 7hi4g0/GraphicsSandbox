#ifndef IMAGE_H
#define IMAGE_H

#include <inttypes.h>

typedef struct {
	void *data;
	uint32_t width;
	uint32_t height;
} Image;

#endif
