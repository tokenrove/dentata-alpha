/*
 * Fading effects for heat
 */

#include <heat.h>
#include <air.h>
#include <flash.h>
#include <quick.h>
#include <stdlib.h>

void heat_flatfadescrimg(flash_image_t *, int, int);
void heat_flatfadein(flash_image_t *, int, int);
void heat_flatfadeout(int, int);

void heat_flatfadescrimg(flash_image_t *after, int fps, int len)
{
	heat_flatfadeout(fps, len);
	heat_flatfadein(after, fps, len);
	return;
}

void heat_flatfadeout(int fps, int len)
{
	unsigned char *palette, *vbuf;
	int steps, step, maxval, i;
	void *qh;

	if(air_mode_type&AIR_8BPP) {
		palette = malloc(3*256);
		steps = len/(1000000/fps);
		if(steps <= 0) steps = 1;
		maxval = 0;
		for(i = 0; i < 256; i++) {
			if(palette[3*i+0] > maxval) maxval = palette[3*i+0];
			if(palette[3*i+1] > maxval) maxval = palette[3*i+1];
			if(palette[3*i+2] > maxval) maxval = palette[3*i+2];
		}
		step = maxval/steps;
		if(step <= 0) step = 1;

		for(steps = 0; steps < (len/(1000000/fps))/2; steps++) {
			qh = quick_start(fps);
			for(i = 0; i < 256; i++) {
				if(palette[3*i+0] > step) palette[3*i+0]-=step;
				else palette[3*i+0] = 0;
				if(palette[3*i+1] > step) palette[3*i+1]-=step;
				else palette[3*i+1] = 0;
				if(palette[3*i+2] > step) palette[3*i+2]-=step;
				else palette[3*i+2] = 0;
			}
			air_setpalette(palette);
			air_update();
			quick_stop(qh);
		}
		air_setpalette(palette);
		free(palette);

	} else if(air_mode_type&AIR_16BPP) {
		steps = len/(1000000/fps);
		if(steps <= 0) steps = 1;
		step = 256/steps;
		if(step <= 0) step = 1;
		vbuf = air_grab();

		for(steps = 0; steps < (len/(1000000/fps))/2; steps++) {
			qh = quick_start(fps);
			for(i = 0; i < air_mode_w*air_mode_h; i++) {
				if(vbuf[2*i+0] > step) vbuf[2*i+0]-=step;
				else vbuf[2*i+0] = 0;
				if(vbuf[2*i+1] > step) vbuf[2*i+1]-=step;
				else vbuf[2*i+1] = 0;
			}
			air_update();
			quick_stop(qh);
		}
	}
	return;
}

void heat_flatfadein(flash_image_t *after, int fps, int len)
{
	unsigned char *palette, *vbuf;
	int steps, step, i, maxval;
	void *qh;

	if(air_mode_type&AIR_8BPP) {
		palette = malloc(3*256);
		steps = len/(1000000/fps);
		if(steps <= 0) steps = 1;
		maxval = 0;
		for(i = 0; i < 256; i++) {
			if(after->palette[3*i+0] > maxval) maxval = after->palette[3*i+0];
			if(after->palette[3*i+1] > maxval) maxval = after->palette[3*i+1];
			if(after->palette[3*i+2] > maxval) maxval = after->palette[3*i+2];
		}
		step = maxval/steps;
		if(step <= 0) step = 1;
		memset(palette, 0, 3*256);

		for(steps = 0; steps < (len/(1000000/fps))/2; steps++) {
			for(i = 0; i < 256; i++) {
				if(palette[3*i+0] < after->palette[3*i+0]-step) palette[3*i+0]+=step;
				else palette[3*i+0] = after->palette[3*i+0];
				if(palette[3*i+1] < after->palette[3*i+1]-step) palette[3*i+1]+=step;
				else palette[3*i+1] = after->palette[3*i+1];
				if(palette[3*i+2] < after->palette[3*i+2]-step) palette[3*i+2]+=step;
				else palette[3*i+2] = after->palette[3*i+2];
			}
			qh = quick_start(fps);
			air_vanillablit(after, 0, 0);
			air_setpalette(palette);
			air_update();
			quick_stop(qh);
		}
		air_setpalette(after->palette);
		free(palette);
	} else if(air_mode_type&AIR_16BPP) {
		steps = len/(1000000/fps);
		if(steps <= 0) steps = 1;
		step = 256/steps;
		if(step <= 0) step = 1;
		vbuf = air_grab();

		for(steps = 0; steps < (len/(1000000/fps))/2; steps++) {
			qh = quick_start(fps);
			for(i = 0; i < air_mode_w*air_mode_h; i++) {
				if(vbuf[2*i+0] < after->data[2*i+0]-step) vbuf[2*i+0]+=step;
				else vbuf[2*i+0] = after->data[2*i+0];
				if(vbuf[2*i+1] < after->data[2*i+1]-step) vbuf[2*i+1]+=step;
				else vbuf[2*i+1] = after->data[2*i+1];
			}
			air_update();
			quick_stop(qh);
		}
	}
	return;
}

/* EOF fade.c */
