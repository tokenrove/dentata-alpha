/*
 * Administrative Functions of air
 * SVGAlib version
 */

#include <air.h>

#include <stdlib.h>
#include <vga.h>
#include "internal.h"

char *air_vbuf, *air_palette;
int air_mode_w, air_mode_h, air_mode_type;

int air_init(int w, int h, int type)
{
	if(w == 320) {
		if(h == 200) {
			if(type&AIR_8BPP) {
				if(vga_setmode(G320x200x256) == 0) {
					air_mode_w = w;
					air_mode_h = h;
					air_mode_type = type;
					air_vbuf = malloc(w*h);
					return 1;
				}
				return 0;
			} else if(type&AIR_16BPP) {
				if(vga_setmode(G320x200x64K) == 0) {
					air_mode_w = w;
					air_mode_h = h;
					air_mode_type = type;
					air_vbuf = malloc(w*h*2);
					return 1;
				}
				return 0;
			} else if(type&AIR_24BPP) {
				if(vga_setmode(G320x200x16M) == 0) {
					air_mode_w = w;
					air_mode_h = h;
					air_mode_type = type;
					air_vbuf = malloc(w*h*3);
					return 1;
				}
				return 0;
			}
		}
	}
	return 0;
}

void air_close(void)
{
	free(air_vbuf);
	vga_setmode(TEXT);
	return;
}

void air_update(void)
{
	int i, inc;
	char *p;

	if(air_mode_type&AIR_8BPP) inc = 1;
	else if(air_mode_type&AIR_16BPP) inc = 2;
	else if(air_mode_type&AIR_24BPP) inc = 3;
	inc *= air_mode_w;
	p = air_vbuf;

	for(i = 0; i < air_mode_h; i++, p+=inc)
		vga_drawscanline(i, p);
}

void air_setpalette(char *palette)
{
	int i;

	for(i = 0; i < 256; i++)
		vga_setpalette(i, palette[i*3], palette[i*3+1],
		               palette[i*3+2]);
	air_palette = palette;
	return;
}

char *air_getpalette(void)
{
	return air_palette;
}

/* EOf admin.c */
