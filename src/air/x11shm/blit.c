/*
 * Blit functions for air
 */

#include <air.h>
#include <flash.h>
#include <drwily.h>
#include <X11/Xlib.h>

#include "internal.h"

void air_blit(flash_image_t *sprite, int x, int y)
{
	if(air_mode_type&AIR_8BPP) {
		drwily_blit8(air_vbuf->data, air_mode_w, air_mode_h,
		             sprite->data, sprite->width, sprite->height,
		             x, y);
	} else if(air_mode_type&AIR_16BPP) {
		if(air_mode_type&AIR_ALPHA1) {
			drwily_blit16alpha1(air_vbuf->data, air_mode_w, air_mode_h,
			                    sprite->data, sprite->alpha,
			                    sprite->width, sprite->height,
			                    x, y);
		} else if(air_mode_type&AIR_ALPHA8) {
			drwily_blit16alpha8(air_vbuf->data, air_mode_w, air_mode_h,
			                    sprite->data, sprite->alpha,
			                    sprite->width, sprite->height,
			                    x, y);
		} else {
			drwily_blit16noalpha(air_vbuf->data, air_mode_w, air_mode_h,
			                     sprite->data, sprite->width,
			                     sprite->height, x, y);
		}
	} else if(air_mode_type&AIR_24BPP) return;
	return;
}

void air_vanillablit(flash_image_t *sprite, int x, int y)
{
	if(air_mode_type&AIR_8BPP) {
		drwily_blit8noalpha(air_vbuf->data, air_mode_w, air_mode_h,
		                    sprite->data, sprite->width,
		                    sprite->height, x, y);
	} else if(air_mode_type&AIR_16BPP) {
		drwily_blit16noalpha(air_vbuf->data, air_mode_w, air_mode_h,
		                     sprite->data, sprite->width,
		                     sprite->height, x, y);
	}
	else if(air_mode_type&AIR_24BPP) return;
	return;
}

unsigned char *air_grab(void)
{
	return air_vbuf->data;
}

/* EOF blit.c */
