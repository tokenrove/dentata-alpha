/*
 * Administrative Functions of air
 * AAlib
 */

#include <air.h>

#include <stdlib.h>
#include <aalib.h>
#include "internal.h"

char *air_vbuf, *air_palette;
int air_mode_w, air_mode_h, air_mode_type;
struct aa_context *air_context;
static aa_palette air_aa_palette;

int air_init(int w, int h, int type)
{
	if(w == 320) {
		if(h == 200) {
			if(type&AIR_8BPP) {
				air_context = aa_autoinit(&aa_defparams);
				if(air_context != NULL) {
					air_mode_w = w;
					air_mode_h = h;
					air_mode_type = type;
					air_vbuf = malloc(w*h);
					return 1;
				}
				return 0;
			} else if(type&AIR_16BPP) {
				return 0;
			} else if(type&AIR_24BPP) {
				return 0;
			}
		}
	}

	return 0;
}

void air_close(void)
{
	free(air_vbuf);
	aa_close(air_context);
	return;
}

#define min(x, y) (((x) > (y)) ? (y) : (x))

void air_update(void)
{
	int i, inc;
	char *p;

	if(air_mode_type&AIR_8BPP) inc = 1;
	else if(air_mode_type&AIR_16BPP) inc = 2;
	else if(air_mode_type&AIR_24BPP) inc = 3;
	inc *= air_mode_w;
	p = air_vbuf;

	for(i = 0; i < air_mode_h && i < aa_imgheight(air_context);
	    i++, p+=inc)
		memcpy(aa_image(air_context)+aa_imgwidth(air_context)*i, p,
		       min(aa_imgwidth(air_context),air_mode_w));

	aa_renderpalette(air_context, air_aa_palette, &aa_defrenderparams,
	                 0, 0, aa_scrwidth(air_context),
	                 aa_scrheight(air_context));
//	aa_fastrender(air_context,
//	                 0, 0, aa_scrwidth(air_context),
//	                 aa_scrheight(air_context));
	aa_flush(air_context);
}

void air_setpalette(char *palette)
{
	int i;

	for(i = 0; i < 256; i++)
		aa_setpalette(air_aa_palette, i, palette[3*i+0]<<2,
		              palette[3*i+1]<<2, palette[3*i+2]<<2);
	air_palette = palette;
	return;
}

char *air_getpalette(void)
{
	return air_palette;
}

/* EOf admin.c */
