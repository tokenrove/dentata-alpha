/*
 * Test software for foo libraries, uses the core 8.
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
#include <bubble.h>
#include <heat.h>

#define FRAMESPERSECOND 24
#define BUFLEN 81
#define SCRW 320
#define SCRH 200
#define SCRTYPE AIR_8BPP

struct gamedat_s {
	crash_font_t *font;
	int hero, head;
};

void scrollie(struct gamedat_s gd);
int actionmenu(struct gamedat_s gd);
void mainloop(struct gamedat_s gd);

void mainloop(struct gamedat_s gd)
{
	int i;
	void *qh;
	bubble_sprite_t *hp;

	hp = bubble_getspritebyhandle(gd.head);
	bubble_spritegencolliderect(hp);
	hp = bubble_getspritebyhandle(gd.hero);
	bubble_spritegencolliderect(hp);

	while(1) {
		metal_update();
		if(metal_ishit(METAL_K_ESCAPE))
			break;
		if(metal_ishit(METAL_K_LEFT)) {
			if(wood_iswalkablerect(hp->x-1, hp->y,
			                       hp->colliderect.w,
			                       hp->colliderect.h)){
				bubble_pansprite(gd.hero, -1, 0);
				if(hp->x-wood_camera_x < SCRW/2)
					wood_pan(-1, 0);
			}
			bubble_setspriteanim(gd.hero, 2);
		}
		if(metal_ishit(METAL_K_RIGHT)) {
			if(wood_iswalkablerect(hp->x+1, hp->y,
			                       hp->colliderect.w,
			                       hp->colliderect.h)){
				bubble_pansprite(gd.hero, 1, 0);
				if(hp->x-wood_camera_x > SCRW/2)
					wood_pan(1, 0);
			}
			bubble_setspriteanim(gd.hero, 3);
		}
		if(metal_ishit(METAL_K_UP)) {
			if(wood_iswalkablerect(hp->x, hp->y-1,
			                       hp->colliderect.w,
			                       hp->colliderect.h)){
				bubble_pansprite(gd.hero, 0, -1);
				if(hp->y-wood_camera_y < SCRH/2)
					wood_pan(0, -1);
			}
			bubble_setspriteanim(gd.hero, 0);
		}
		if(metal_ishit(METAL_K_DOWN)) {
			if(wood_iswalkablerect(hp->x, hp->y+1,
			                       hp->colliderect.w,
			                       hp->colliderect.h)){
				bubble_pansprite(gd.hero, 0, 1);
				if(hp->y-wood_camera_y > SCRH/2)
					wood_pan(0, 1);
			}
			bubble_setspriteanim(gd.hero, 1);
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
		bubble_updatespr();
		wood_updatefg();

		air_update();
		quick_stop(qh);
	}
	return;
}

int main(int argc, char **argv)
{
	struct gamedat_s gd;

	if(air_init(SCRW, SCRH, SCRTYPE) != 1) exit(EXIT_FAILURE);

	if(metal_init() != 1) {
		air_close();
		exit(EXIT_FAILURE);
	}

	gd.font = crash_loadrawfont("future.f14", 8, 14, 23);
	scrollie(gd);
	crash_delete(gd.font);
	gd.font = crash_loadrawfont("readable.f08", 8, 8,
	                            flash_closestcolor(255, 255, 255,
	                                               air_getpalette()));

	wood_wipe(SCRW, SCRH, AIR_8BPP);
	wood_addtilemaptobg(wood_loadtilemap("world1-1.map"), 1);
	bubble_init();
	gd.hero = bubble_addsprite(bubble_spriteload("hero.spr"), 1);
	gd.head = bubble_addsprite(bubble_spriteload("head.spr"), 2);

	mainloop(gd);

	bubble_close();
	wood_wipe(0, 0, 0);
	crash_delete(gd.font);
	metal_close();
	air_close();
	exit(EXIT_SUCCESS);
}

#define max(x, y) (((x) < (y)) ? (y) : (x))

int actionmenu(struct gamedat_s gd)
{
	int pos = 0, x = 0, w, h;
	char menu[2][5] = { "Talk", "Quit" };
	void *qh;

	w = 4+(gd.font->width+1)*(strlen(menu[0])+1);
	w = max(w, 4+(gd.font->width+1)*(strlen(menu[1])+1));
	h = 4+(gd.font->height+1)*2;

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

		heat_box(100-gd.font->width-1, 40-1, w, h, flat,
		         flash_closestcolor(255,255,255,air_getpalette()),
		         flash_closestcolor(0,0,255,air_getpalette()));
		crash_printf(100, 40+1, gd.font, menu[0]);
		crash_printf(100, 40+gd.font->height+1, gd.font, menu[1]);
		if((x/6)%2 == 0)
			crash_printf(100-gd.font->width,
			             40+1+(pos*gd.font->height),
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

	wood_wipe(SCRW, SCRH, AIR_8BPP);
	bganim = flash_animnew(10);
	flash_animaddframe(bganim, flash_loadpcx("testbg00.pcx"));
	air_setpalette(bganim->frames[0]->palette);
	flash_animaddframe(bganim, flash_loadpcx("testbg01.pcx"));
	flash_animaddframe(bganim, flash_loadpcx("testbg02.pcx"));
	flash_animaddframe(bganim, flash_loadpcx("testbg02.pcx"));
	flash_animaddframe(bganim, flash_loadpcx("testbg02.pcx"));
	flash_animaddframe(bganim, flash_loadpcx("testbg02.pcx"));
	wood_addanimtobg(bganim, 2);

	sprx = SCRW;
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
		if(sprx < -(signed)strlen(msg)*(gd.font->width+1))
			sprx = SCRW;

		crash_printf(sprx, SCRH-(gd.font->height+2), gd.font, msg);

		air_update();
		quick_stop(qh);
	}

	return;
}

/* test3.c */
