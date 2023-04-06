#ifndef IMAGE_H
#define IMAGE_H

#include <inttypes.h>

typedef union {
	uint32_t value;
	struct {
		uint8_t B;
		uint8_t G;
		uint8_t R;
		uint8_t A;
	};
} Pixel;

typedef struct {
	void *data;
	uint32_t width;
	uint32_t height;
} Image;

extern Pixel alphaBlend(Pixel src, Pixel dst);
extern int saveImage(Image image, char fileName[]);

#endif
