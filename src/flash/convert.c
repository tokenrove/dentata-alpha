/*
 * Conversion component of flash
 */

#include <flash.h>
#include <air.h>
#include <stdlib.h>

flash_image_t *flash_imgtypeconvert(flash_image_t *orig, int type)
{
	flash_image_t *p;
	int i;
	unsigned char r, g, b;

	p = flash_imgnew(orig->width, orig->height, type);
	if(orig->type&AIR_8BPP) {
/*		if(orig->type&AIR_GRAYSCALE) {
		} else if(orig->type&AIR_PALETTED) {
		} else if(orig->type&AIR_RGB) {
		} */
		if(type&AIR_8BPP) {
			flash_imgdelete(p);
			return orig;
		} else if(type&AIR_16BPP) {
			if(type&AIR_ALPHA1)
				memset(p->alpha, 0, (p->width*p->height+7)/8);
			for(i = 0; i < p->width*p->height; i++) {
				if(type&AIR_ALPHA1) {
//					p->alpha[i/8] = (p->alpha[i/8]&~(1<<i))|(orig->data[i]?1:0)<<(i%8);
					p->alpha[i/8] |= (orig->data[i]?1:0)<<(i%8);
				} else if(type&AIR_ALPHA8) {
					p->alpha[i] = orig->data[i]?255:0;
				}
				r = orig->palette[3*orig->data[i]+0];
				g = orig->palette[3*orig->data[i]+1];
				b = orig->palette[3*orig->data[i]+2];
				p->data[2*i+1] = (r&~7)|(g>>5);
				p->data[2*i+0] = ((g<<3)&~63)|(b>>3);
			}
		} else if(type&AIR_24BPP) {
			for(i = 0; i < p->width*p->height; i++) {
				if(type&AIR_ALPHA1) {
					p->alpha[i/8] = (p->alpha[i/8]&(~1<<i))|(orig->data[i]?1:0);
				} else if(orig->data[i] == 0 && type&AIR_ALPHA8) {
					p->alpha[i] = orig->data[i]?255:0;
				}
				r = orig->palette[3*orig->data[i]+0];
				g = orig->palette[3*orig->data[i]+1];
				b = orig->palette[3*orig->data[i]+2];
				p->data[3*i+0] = r;
				p->data[3*i+1] = g;
				p->data[3*i+2] = b;
			}
		}
	} else if(orig->type&AIR_16BPP) {
	} else {
		return NULL;
	}

	flash_imgdelete(orig);
	return p;
}

/* EOF convert.c */
