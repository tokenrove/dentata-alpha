/*
 * Administrative Functions of air
 * DOS version
 */

#include <air.h>

#include <stdlib.h>
#include <go32.h>
#include <dpmi.h>
#include <dos.h>
#include <pc.h>
#include <sys/farptr.h>
#include "internal.h"

char *air_vbuf;
unsigned char *air_palette;
int air_mode_w, air_mode_h, air_mode_type;

int air_init(int w, int h, int type)
{
	union REGS r;

	if(w == 320) {
		if(h == 200) {
			if(type&AIR_8BPP) {
				r.x.ax = 0x13;
				int86(0x10, &r, &r);
				air_mode_w = w;
				air_mode_h = h;
				air_mode_type = type;
				air_vbuf = malloc(w*h);
				return 1;
			}
		}
	}
	return 0;
}

void air_close(void)
{
	union REGS r;
	free(air_vbuf);
	r.x.ax = 0x03;
	int86(0x10, &r, &r);
	return;
}

void air_update(void)
{
	int i, j, inc;
	char *p;

	if(air_mode_type&AIR_8BPP) inc = 1;
	else if(air_mode_type&AIR_16BPP) inc = 2;
	else if(air_mode_type&AIR_24BPP) inc = 3;
	inc *= air_mode_w;
	p = air_vbuf;

	if(air_mode_type&AIR_8BPP && air_mode_w == 320)
		movedata(_my_ds(), air_vbuf,
		         __dpmi_segment_to_descriptor(0xa000), 0,
		         air_mode_w*air_mode_h);

	return;
}

void air_setpalette(unsigned char *palette)
{
	int i;

	outb(0x3c8, 0);
	for(i = 0; i < 256*3; i++)
		outb(0x3c9, palette[i]);
	air_palette = palette;
	return;
}

unsigned char *air_getpalette(void)
{
	return air_palette;
}

/* EOf admin.c */
