#include <Image/image.h>

Pixel alphaBlend(Pixel src, Pixel dst) {
	Pixel out;
	uint16_t invAlpha;

	invAlpha = 0xFF ^ src.A;

	out.R = (src.R * (uint16_t) src.A + dst.R * invAlpha) >> 8;
	out.G = (src.G * (uint16_t) src.A + dst.G * invAlpha) >> 8;
	out.B = (src.B * (uint16_t) src.A + dst.B * invAlpha) >> 8;

	return out;
}
