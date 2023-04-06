#include <stdio.h>
#include <stdlib.h>

#include <Image/image.h>

char resultFilename[] = "result.ppm";

int main(int argc, char* argv[]) {
	FILE *first, *second;
	char magic[3];
	int colorDepth;
	int32_t maxDiff;
	float scaleFactor;
	Image image = {};
	Pixel *pixel;

	if (argc < 3) {
		fprintf(stderr, "Usage: %s <file1.ppm> <file2.ppm>\n", argv[0]);
		exit(1);
	}
	first = fopen(argv[1], "r");
	second = fopen(argv[2], "r");

	if (first == NULL || second == NULL) {
		fprintf(stderr, "Failed to open files\n");
		exit(1);
	}

	fgets(magic, sizeof(magic), first);
	if (magic[0] != 'P' || magic[1] != '3') {
		fprintf(stderr, "Invalid PPM file format for %s\n", argv[1]);
		exit(1);
	}

	fgets(magic, sizeof(magic), second);
	if (magic[0] != 'P' || magic[1] != '3') {
		fprintf(stderr, "Invalid PPM file format for %s\n", argv[2]);
		exit(1);
	}

	fscanf(first, "%d %d %d", &image.width, &image.height, &colorDepth);
	if (colorDepth != 255) {
		fprintf(stderr, "Unexpected color depth of %d\n", colorDepth);
		exit(1);
	}
	{
		int cmpHeight, cmpWidth;
		fscanf(second, "%d %d %d", &cmpWidth, &cmpHeight, &colorDepth);

		if (image.width != cmpWidth || image.height != cmpHeight) {
			fprintf(stderr, "Mismatched sizes (%d %d) and (%d %d)\n", image.width, image.height, cmpWidth, cmpHeight);
			exit(1);
		}

		if (colorDepth != 255) {
			fprintf(stderr, "Unexpected color depth of %d\n", colorDepth);
			exit(1);
		}
	}

	image.data = malloc(sizeof(Pixel) * image.width * image.height);
	if (image.data == NULL) {
		fprintf(stderr, "Unable to allocate memory\n");
		exit(1);
	}

	pixel = (Pixel*) image.data;
	maxDiff = 0;

	for (int y = 0; y < image.height; y++) {
		for (int x = 0; x < image.width; x++) {
			int32_t R, G, B;
			fscanf(second, "%hhu %hhu %hhu", &pixel->R, &pixel->G, &pixel->B);
			fscanf(first, "%d %d %d", &R, &G, &B);

			R -= pixel->R;
			G -= pixel->G;
			B -= pixel->B;

			if (abs(R) > maxDiff) {
				maxDiff = abs(R);
			}
			if (abs(G) > maxDiff) {
				maxDiff = abs(G);
			}
			if (abs(B) > maxDiff) {
				maxDiff = abs(B);
			}

			pixel->R = (uint8_t) R;
			pixel->G = (uint8_t) G;
			pixel->B = (uint8_t) B;

			pixel++;
		}
		fgetc(first);
		fgetc(second);
	}

	pixel = (Pixel*) image.data;
	scaleFactor = 127.0f/maxDiff;
	for (int y = 0; y < image.height; y++) {
		for (int x = 0; x < image.width; x++) {

			pixel->R = (uint8_t) (((int16_t) pixel->R) * scaleFactor) + 128;
			pixel->G = (uint8_t) (((int16_t) pixel->G) * scaleFactor) + 128;
			pixel->B = (uint8_t) (((int16_t) pixel->B) * scaleFactor) + 128;

			pixel++;
		}
	}

	if (saveImage(image, resultFilename) == 0) {
		fprintf(stderr, "File saved!\n");
	}

	fclose(first);
	fclose(second);

	free(image.data);
}