/*
 * Palette functions for flash
 */

#include <air.h>
#include <flash.h>
#include <stdio.h>
#include <stdlib.h>

void flash_loadpalette(char *, flash_image_t *);
int flash_closestcolor(int, int, int);
int flash_closestcolor16(int, int, int);
int flash_closestcolor8(int, int, int, unsigned char *);

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

int flash_closestcolor(int r, int g, int b)
{
	if(air_mode_type&AIR_16BPP)
		return flash_closestcolor16(r, g, b);
	else if(air_mode_type&AIR_8BPP)
		return flash_closestcolor8(r, g, b, air_getpalette());
	return 0;
}

int flash_closestcolor16(int r, int g, int b)
{
	return ((((g<<3)&~31)<<8)|(b>>3))|(((r<<3)&~7)|(g>>5));
}


int flash_closestcolor8(int r, int g, int b, unsigned char *palette)
{
	int i, j, rp, gp, bp, xbar, ybar;

	rp = palette[0]; gp = palette[1]; bp = palette[2];

	for(i = 0, j = 0; i < 256; i++) {
		xbar = abs(r-palette[3*i+0])+
		       abs(g-palette[3*i+1])+
		       abs(b-palette[3*i+2]);
		xbar /= 3;
		ybar = abs(g-gp)+abs(b-bp)+abs(r-rp);
		ybar /= 3;

		if(xbar < ybar) {
			j = i;
			rp = palette[3*i+0];
			gp = palette[3*i+1];
			bp = palette[3*i+2];
		}
	}

	return j;
}

/* EOF palette.c */
