/*
 * Palette functions for flash
 */

#include <air.h>
#include <flash.h>
#include <stdio.h>
#include <stdlib.h>

void flash_loadpalette(char *, flash_image_t *);
int flash_closestcolor(int, int, int, unsigned char *);

void flash_loadpalette(char *filename, flash_image_t *p)
{
	FILE *fp;

	fp = fopen(filename, "rb");
	if(fp == NULL) return;

	p->palette = malloc(3*256);
	if(p->palette == NULL) return;
	fread(p->palette, 256, 3, fp);
	fclose(fp);
	return;
}

#define abs(x) ((x)>0?(x):-(x))

int flash_closestcolor(int r, int g, int b, unsigned char *palette)
{
	int i, j, rp, gp, bp, xbar, ybar;

	rp = palette[0]<<2; gp = palette[1]<<2; bp = palette[2]<<2;

	for(i = 0, j = 0; i < 256; i++) {
		xbar = abs(r-(palette[3*i+0]<<2))+
		       abs(g-(palette[3*i+1]<<2))+
		       abs(b-(palette[3*i+2]<<2));
		xbar /= 3;
		ybar = abs(g-gp)+abs(b-bp)+abs(r-rp);
		ybar /= 3;

		if(xbar < ybar) {
			j = i;
			rp = palette[3*i+0]<<2;
			gp = palette[3*i+1]<<2;
			bp = palette[3*i+2]<<2;
		}
	}

	return j;
}

/* EOF palette.c */
