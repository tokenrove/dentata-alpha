/*
 * Test software for foo libraries.
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

#define FRAMESPERSECOND 24
#define BUFLEN 81

struct gamedat_s {
	crash_font_t *font;
	flash_anim_t *hero[4], *head;
};

void scrollie(struct gamedat_s gd);
int actionmenu(struct gamedat_s gd);
void mainloop(struct gamedat_s gd);

void mainloop(struct gamedat_s gd)
{
	int spdir = 0;
	int sprx = 160, spry = 100, headx = 10, heady = 10, sprw, sprh, i;
	void *qh;

	sprw = gd.hero[spdir]->frames[0]->width;
	sprh = gd.hero[spdir]->frames[0]->height;

	while(1) {
		metal_update();
		if(metal_ishit(METAL_K_ESCAPE))
			break;
		if(metal_ishit(METAL_K_LEFT)) {
			if(wood_tiletype(sprx-1, spry) < 2 &&
			   wood_tiletype(sprx-1, spry+sprh-1) < 2) {
				if(sprx > 320/2) sprx--;
				else if(wood_pan(-1, 0)&1 && sprx >= 0)
					sprx--;
			}
			spdir = 2;
		}
		if(metal_ishit(METAL_K_RIGHT)) {
			if(wood_tiletype(sprx+sprw, spry) < 2 &&
			   wood_tiletype(sprx+sprw, spry+sprh-1) < 2) {
				if(sprx < 320/2) sprx++;
				else if(wood_pan(1, 0)&1 && sprx < 320)
					sprx++;
			}
			spdir = 3;
		}
		if(metal_ishit(METAL_K_UP)) {
			if(wood_tiletype(sprx, spry-1) < 2 &&
			   wood_tiletype(sprx+sprw-1, spry-1) < 2) {
				if(spry > 200/2) spry--;
				else if(wood_pan(0, -1)&2 && spry >= 0)
					spry--;
			}
			spdir = 0;
		}
		if(metal_ishit(METAL_K_DOWN)) {
			if(wood_tiletype(sprx, spry+sprh) < 2 &&
			   wood_tiletype(sprx+sprw-1, spry+sprh) < 2) {
				if(spry < 200/2) spry++;
				else if(wood_pan(0, 1)&2 && spry < 200)
					spry++;
			}
			spdir = 1;
		}
		if(metal_ishit(METAL_K_ENTER)) {
			i = actionmenu(gd);
			if(i == 0) { /* talk to */
			} else if(i == 1) { /* quit */
				break;
			}
		}

		qh = quick_start(FRAMESPERSECOND);

		wood_updatebg();
		air_blit(flash_animnextframe(gd.hero[spdir]), sprx, spry);
		air_blit(flash_animnextframe(gd.head), headx, heady);
		heady ^= 1;
		wood_updatefg();

		air_update();
		quick_stop(qh);
	}
	return;
}

int main(int argc, char **argv)
{
	struct gamedat_s gd;
	int i;

	if(air_init(320, 200, AIR_8BPP) != 1) exit(EXIT_FAILURE);

	if(metal_init() != 1) {
		air_close();
		exit(EXIT_FAILURE);
	}

	gd.font = crash_loadrawfont("future.f14", 8, 14, 23);
	scrollie(gd);
	crash_delete(gd.font);
	gd.font = crash_loadrawfont("readable.f08", 8, 8, 43);
/*	                            flash_closestcolor(255, 255, 255,
	                                               air_getpalette())); */

	wood_wipe(320, 200, AIR_8BPP);
	wood_addtilemaptobg(wood_loadtilemap("world1-1.map"), 1);
	for(i = 0; i < 4; i++)
		gd.hero[i] = flash_animnew(6);
	gd.head = flash_animnew(6);

	flash_animaddframe(gd.hero[0], flash_loadpcx("spwlkb00.pcx"));
	flash_animaddframe(gd.hero[0], flash_loadpcx("spwlkb01.pcx"));
	flash_animaddframe(gd.hero[1], flash_loadpcx("spwlkf00.pcx"));
	flash_animaddframe(gd.hero[1], flash_loadpcx("spwlkf01.pcx"));
	flash_animaddframe(gd.hero[2], flash_loadpcx("spwlkl00.pcx"));
	flash_animaddframe(gd.hero[3], flash_loadpcx("spwlkr00.pcx"));
	flash_animaddframe(gd.hero[3], flash_loadpcx("spwlkr01.pcx"));

	flash_animaddframe(gd.head, flash_loadpcx("head00.pcx"));
	flash_animaddframe(gd.head, flash_loadpcx("head01.pcx"));
	flash_animaddframe(gd.head, flash_loadpcx("head02.pcx"));

	mainloop(gd);

	for(i = 0; i < 4; i++)
		flash_animdelete(gd.hero[i]);
	flash_animdelete(gd.head);
	wood_wipe(0, 0, 0);
	crash_delete(gd.font);
	metal_close();
	air_close();
	exit(EXIT_SUCCESS);
}

int actionmenu(struct gamedat_s gd)
{
	int pos = 0, x = 0;
	void *qh;

	while(metal_ishit(METAL_K_ENTER)) metal_update();
	while(1) {
		metal_update();
		if(metal_ishit(METAL_K_DOWN))
			if(pos < 1) pos++;
		if(metal_ishit(METAL_K_UP))
			if(pos > 0) pos--;
		if(metal_ishit(METAL_K_ENTER))
			break;

		qh = quick_start(FRAMESPERSECOND);

		crash_printf(100, 40, gd.font, "talk to");
		crash_printf(100, 40+gd.font->height, gd.font, "quit");
		if((x/6)%2 == 0)
			crash_printf(100-gd.font->width,
			             40+(pos*gd.font->height),
		                     gd.font, ">");
		x++;

		air_update();
		quick_stop(qh);
	}
	while(metal_ishit(METAL_K_ENTER)) metal_update();
	return pos;
}

void scrollie(struct gamedat_s gd)
{
	int sprx;
	void *qh;
	flash_anim_t *bganim;
	char *msg = "The long anticipated ``test3'' by Tek... Greetings "
"go out to the mighty Retsyn for starting all this, Obliviax for ripping "
"NES graphics and beta testing, Dum2007 and Astfgl for beta testing and "
"moral support, and finally Kahless for not being here... Keep "
"on dancing! [oh, and hit enter to go to the tiled world]";

	wood_wipe(320, 200, AIR_8BPP);
	bganim = flash_animnew(10);
	flash_animaddframe(bganim, flash_loadpcx("testbg00.pcx"));
	air_setpalette(bganim->frames[0]->palette);
	flash_animaddframe(bganim, flash_loadpcx("testbg01.pcx"));
	flash_animaddframe(bganim, flash_loadpcx("testbg02.pcx"));
	flash_animaddframe(bganim, flash_loadpcx("testbg02.pcx"));
	flash_animaddframe(bganim, flash_loadpcx("testbg02.pcx"));
	flash_animaddframe(bganim, flash_loadpcx("testbg02.pcx"));
	wood_addanimtobg(bganim, 2);

	sprx = 320;
	while(1) {
		metal_update();
		if(metal_ishit(METAL_K_ESCAPE))
			break;
		else if(metal_ishit(METAL_K_ENTER))
			break;

		qh = quick_start(FRAMESPERSECOND);

		wood_updatebg();
		wood_updatefg();

		sprx-=2;
		if(sprx < -(signed)strlen(msg)*9) sprx = 320;

		crash_printf(sprx, 200-16, gd.font, msg);

		air_update();
		quick_stop(qh);
	}

	return;
}

/* test3.c */
