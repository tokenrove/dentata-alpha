/*
 * Test software for foo libraries, like test1 but this time using
 * additionally the crash and wood libraries.
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
#include <wood.h>
#include <crash.h>

#define NSTANDFRAMES 4
#define NWALKFRAMES 4
#define FRAMESPERSECOND 24
#define SCRW 320
#define SCRH 200
#define BUFLEN 81

struct gamedat_s {
	flash_image_t *bg;
	flash_anim_t *spr_stand, *spr_walkleft, *spr_walkright;
	crash_font_t *font;
};

void mainloop(struct gamedat_s gd)
{
	int sprx, spry, fps;
	void *qh;
	flash_anim_t *sprite;

	fps = FRAMESPERSECOND;
	sprx = SCRW/2; spry = SCRH/2;
	sprite = gd.spr_stand;

	while(1) {
		metal_update();
		if(metal_ishit(METAL_K_ESCAPE))
			break;
		if(metal_ishit(METAL_K_UP)) {
			if(spry > SCRH/2)
				spry--;
			else
				if(wood_pan(0, -1)&2)
					if(spry >= 0) spry--;
		}
		if(metal_ishit(METAL_K_DOWN)) {
			if(spry < SCRH/2)
				spry++;
			else
				if(wood_pan(0, 1)&2)
					if(spry < SCRH) spry++;
		}
		if(metal_ishit(METAL_K_LEFT)) {
			sprite = gd.spr_walkleft;
			if(sprx > SCRW/2)
				sprx--;
			else
				if(wood_pan(-1, 0)&1)
					if(sprx >= 0) sprx--;
		} else if(metal_ishit(METAL_K_RIGHT)) {
			sprite = gd.spr_walkright;
			if(sprx < SCRW/2)
				sprx++;
			else
				if(wood_pan(1, 0)&1)
					if(sprx < SCRW) sprx++;
		} else
			sprite = gd.spr_stand;

		qh = quick_start(fps);

		wood_updatebg();
		air_blit(flash_animnextframe(sprite), sprx, spry);
		wood_updatefg();

		crash_printf(sprx, gd.font->height, gd.font, "Foobar %d, %d", sprx, spry);

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

	if(air_init(SCRW, SCRH, AIR_8BPP) != 1) exit(EXIT_FAILURE);

	if(metal_init() != 1) {
		air_close();
		exit(EXIT_FAILURE);
	}

	wood_wipe(SCRW, SCRH, AIR_8BPP);
	gd.bg = flash_loadpcx("testbg00.pcx");
	air_setpalette(gd.bg->palette);

//	gd.font = crash_loadrawfont("readable.f08", 8, 8, 249);
	gd.font = crash_loadrawfont("future.f14", 8, 14,
	                            flash_closestcolor(255, 255, 255,
	                                               air_getpalette()));


	wood_addspritetobg(gd.bg, 4);
	wood_addtilemaptobg(wood_loadtilemap("flatwrld.map"), 1);
	if(argc > 1 && strcmp(argv[1], "-fg") == 0)
		wood_addspritetofg(flash_loadpcx("testbg02.pcx"), 2);
	gd.spr_stand = flash_animnew(2);
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

//	wood_wipe(0, 0, 0);
	flash_animdelete(gd.spr_stand);
	flash_animdelete(gd.spr_walkleft);
	flash_animdelete(gd.spr_walkright);
	crash_delete(gd.font);
	metal_close();
	air_close();
	exit(EXIT_SUCCESS);
}

/* test2.c */
