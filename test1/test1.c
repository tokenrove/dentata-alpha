/*
 * Test software for foo libraries, testing the first four villains.
 * [air, flash, metal, quick]
 *
 * Julian Squires <tek@wiw.org> 2000
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <air.h>
#include <flash.h>
#include <quick.h>
#include <metal.h>

#define NSTANDFRAMES 4
#define NWALKFRAMES 4
#define FRAMESPERSECOND 24
#define BUFLEN 81

struct gamedat_s {
	flash_image_t *bg;
	flash_anim_t *spr_stand, *spr_walkleft, *spr_walkright;
};

void mainloop(struct gamedat_s gd)
{
	int sprx, spry, radius, fps, mode;
	float theta;
	void *qh;
	flash_anim_t *sprite;

	fps = FRAMESPERSECOND;
	theta = 0;
	radius = 8;
	mode = 1;
	sprx = 100; spry = 100;
	sprite = gd.spr_stand;

	while(1) {
		metal_update();
		if(metal_ishit(METAL_K_ESCAPE))
			break;
		if(metal_ishit(METAL_K_UP)) {
			if(mode == 0)
				radius++;
			else
				spry--;
		}
		if(metal_ishit(METAL_K_DOWN)) {
			if(mode == 0)
				radius--;
			else
				spry++;
		}
		if(metal_ishit(METAL_K_LEFT)) {
			sprite = gd.spr_walkleft;
			if(mode == 0)
				theta -= 0.615;
			else
				sprx--;
		} else if(metal_ishit(METAL_K_RIGHT)) {
			sprite = gd.spr_walkright;
			if(mode == 0)
				theta += 0.615;
			else
				sprx++;
		} else if(metal_ishit(METAL_K_ENTER)) {
			mode ^= 1;
		} else
			sprite = gd.spr_stand;

		qh = quick_start(fps);

		if(mode == 0) {
			sprx = 100+radius*cos(theta);
			spry = 100+radius*sin(theta);
		}

		air_vanillablit(gd.bg, 0, 0);
		air_blit(flash_animnextframe(sprite), sprx, spry);

		air_update();
		quick_stop(qh);
	}

	return;
}

int main(int argc, char **argv)
{
	char buffer[BUFLEN];
	struct gamedat_s gd;
	int i;

	if(air_init(320, 200, AIR_8BPP) != 1) exit(EXIT_FAILURE);

	if(metal_init() != 1) {
		air_close();
		exit(EXIT_FAILURE);
	}

	gd.bg = flash_loadpcx("testbg.pcx");
	air_setpalette(gd.bg->palette);
	gd.spr_stand = flash_animnew(5);
	gd.spr_walkleft = flash_animnew(2);
	gd.spr_walkright = flash_animnew(2);

	if(gd.bg == NULL || gd.spr_stand == NULL || gd.spr_walkleft == NULL ||
	   gd.spr_walkright == NULL) {
		metal_close();
		air_close();
		exit(EXIT_FAILURE);
	}

	for(i = 0; i < NSTANDFRAMES; i++) {
		sprintf(buffer, "spstnd%02d.pcx", i);
		flash_animaddframe(gd.spr_stand, flash_loadpcx(buffer));
	}
	for(i = 0; i < NWALKFRAMES; i++) {
		sprintf(buffer, "spwlkl%02d.pcx", i);
		flash_animaddframe(gd.spr_walkleft, flash_loadpcx(buffer));
		sprintf(buffer, "spwlkr%02d.pcx", i);
		flash_animaddframe(gd.spr_walkright, flash_loadpcx(buffer));
	}

	mainloop(gd);

	flash_imgdelete(gd.bg);
	flash_animdelete(gd.spr_stand);
	flash_animdelete(gd.spr_walkleft);
	flash_animdelete(gd.spr_walkright);
	metal_close();
	air_close();
	exit(EXIT_SUCCESS);
}

/* test1.c */
