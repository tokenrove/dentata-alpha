/*
 * Image management for flash
 */

#include <flash.h>
#include <air.h>
#include <drwily.h>

#include <stdlib.h>

flash_image_t *flash_imgnew(int, int, int);
void flash_imgdelete(flash_image_t *);
flash_image_t *flash_imgdup(flash_image_t *);
void flash_imgblit(flash_image_t *, flash_image_t *, int, int);

flash_image_t *flash_imgnew(int w, int h, int type)
{
	flash_image_t *p;
	int inc;

	p = malloc(sizeof(flash_image_t));
	if(p == NULL)
		return NULL;

	p->width = w;
	p->height = h;
	p->type = type;
	if(type&AIR_8BPP) inc = 1;
	else if(type&AIR_16BPP) inc = 2;
	else if(type&AIR_24BPP) inc = 3;
	p->data = malloc(w*h*inc);
	if(type&AIR_ALPHA1) {
		p->alpha = malloc((w*h+7)/8);
	} else if(type&AIR_ALPHA8) {
		p->alpha = malloc(w*h);
	} else {
		p->alpha = NULL;
	}
	p->palette = NULL;
	p->palettetype = 0;

	return p;
}

void flash_imgdelete(flash_image_t *p)
{
	if(p->data) free(p->data);
	if(p->alpha) free(p->alpha);
	if(p->palette) free(p->palette);
	return;
}

flash_image_t *flash_imgdup(flash_image_t *p)
{
	flash_image_t *q;
	int inc;

	q = flash_imgnew(p->width, p->height, p->type);
	if(p->type&AIR_8BPP) inc = 1;
	else if(p->type&AIR_16BPP) inc = 2;
	else if(p->type&AIR_24BPP) inc = 3;
	memcpy(q->data, p->data, p->width*p->height*inc);
	return q;
}

void flash_imgblit(flash_image_t *dest, flash_image_t *src, int x, int y)
{
	if(dest->type != src->type) {
		/* do something uncouth */
	}
	if(dest->type&AIR_8BPP) {
		drwily_blit8(dest->data, dest->width, dest->height,
		             src->data, src->width, src->height, x, y);
	} else if(dest->type&AIR_16BPP) {
		if(dest->type&AIR_ALPHA1) {
			drwily_blit16alpha1(dest->data, dest->width,
			                    dest->height, src->data,
			                    src->alpha, src->width,
			                    src->height, x, y);
		} else if(dest->type&AIR_ALPHA8) {
			drwily_blit16alpha8(dest->data, dest->width,
			                    dest->height, src->data,
			                    src->alpha, src->width,
			                    src->height, x, y);
		} else {
			return;
		}
	} else if(dest->type&AIR_24BPP) return;
	return;
}
/* EOF image.c */
