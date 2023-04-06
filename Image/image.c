#include <stdio.h>

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

int saveImage(Image image, char fileName[]) {
	FILE *imageFile = fopen(fileName, "w");
	Pixel *pixel;
	int result;

	if (imageFile == NULL) {
		return 1;
	}

	result = fprintf(imageFile, "P3\n%d %d\n255\n", image.width, image.height);
	if (result == EOF) {
		return 1;
	}

	pixel = (Pixel *) image.data;

	for (int y = 0; y < image.height; y++) {
		for (int x = 0; x < image.width; x++) {
			result = fprintf(imageFile, "%d %d %d ", pixel->R, pixel->G, pixel->B);
			if (result == EOF) {
				return 1;
			}
			pixel++;
		}
		result = fprintf(imageFile, "\n");
		if (result == EOF) {
			return 1;
		}
	}

	result = fclose(imageFile);
	if (result != 0) {
		return 1;
	}

	return 0;
}
