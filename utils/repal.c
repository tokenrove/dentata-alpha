/*
 * repalette
 * Changes the palette on a PCX, keeping the colors as close
 * to the original as possible given the new palette.
 */

#include <stdio.h>
#include <stdlib.h>
#include <flash.h>

int main(int argc, char **argv)
{
	flash_image_t *img, *newpalimg;
	int i, run;
	FILE *fp;

	if(argc != 3) {
		printf("Usage:\nrepalette <PCX to change> <PCX from "
		       "which to obtain new palette>\n");
		exit(EXIT_FAILURE);
	}

	img = flash_loadpcx(argv[1]);
	newpalimg = flash_loadpcx(argv[2]);
	if(img == NULL || newpalimg == NULL) {
		printf("Failed to load one of the images.\n");
		exit(EXIT_FAILURE);
	}

	for(i = 0; i < img->width*img->height; i++) {
		img->data[i] = flash_closestcolor8(img->palette[3*(unsigned)img->data[i]+0],
		                                   img->palette[3*(unsigned)img->data[i]+1],
		                                   img->palette[3*(unsigned)img->data[i]+2],
		                                   newpalimg->palette);
	}

	fp = fopen(argv[1], "r+b");
	if(fp == NULL) {
		printf("Failed to open %s. (Something very bad has just "
                       "occurred)\n", argv[1]);
	}
	fseek(fp, 128, SEEK_SET);
	for(i = 0, run = 0; i < img->width*img->height; i++) {
		if(i < img->width*img->height-1 &&
		   img->data[i+1] == img->data[i])
			run++;
		else {
			if(run > 0) {
				while(run > 63) {
					fputc(0xC0+63, fp);
					fputc(img->data[i], fp);
					run -= 63;
				}
				if(run > 0) {
					fputc(0xC0|(run+1), fp);
					fputc(img->data[i], fp);
				}
				run = 0;
			} else {
				if((unsigned)img->data[i] > 0xBF) {
					fputc(0xC1, fp);
					fputc(img->data[i], fp);
				} else {
					fputc(img->data[i], fp);
				}
			}
		}
	}
	if(run > 0) {
		while(run > 64) {
			fputc(0xC0+64, fp);
			fputc(img->data[i], fp);
			run -= 64;
		}
		if(run > 0) {
			fputc(0xC0+run, fp);
			fputc(img->data[i], fp);
		}
		run = 0;
	}

	fseek(fp, -768, SEEK_END);

	for(i = 0; i < 256; i++) {
		fputc(newpalimg->palette[3*i+0], fp);
		fputc(newpalimg->palette[3*i+1], fp);
		fputc(newpalimg->palette[3*i+2], fp);
	}
	fclose(fp);

	flash_imgdelete(img);
	flash_imgdelete(newpalimg);

	exit(EXIT_SUCCESS);
}

/* EOF repalette.c */
