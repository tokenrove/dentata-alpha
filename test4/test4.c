/*
 * Super Test4 Alpha Turbo
 */

#include <air.h>
#include <bubble.h>
#include <metal.h>
#include <crash.h>
#include <quick.h>
#include <heat.h>
#include <flash.h>
#include <wood.h>

#include <stdio.h>
#include <stdlib.h>

#define SCRW 320
#define SCRH 200
#define FRAMESPERSECOND 72
#define FLOORY 180
#define VXMAX 4
#define VXACCEL 1
#define VXSTEP 2
#define VYMAX 10
#define VYACCEL 1
#define VYSTEP 4
#define XPANPOS 10
#define YPANPOS 10
#define XPUSH 5

#define FADETIME 1000000

#define ANIMWALKFR 0
#define ANIMWALKFL 1
#define ANIMJUMPFR 2
#define ANIMJUMPFL 3
#define ANIMDUCKFR 4
#define ANIMDUCKFL 5

struct playerdata_s {
	int vx, vy;
	int face, duck;
};

struct gamedata_s {
	bubble_sprite_t *pl1;
	struct playerdata_s pl1sup;
	int pl1handle;
	bubble_collidemode_t p1collide;

	bubble_sprite_t *pl2;
	struct playerdata_s pl2sup;
	int pl2handle;
	bubble_collidemode_t p2collide;

	crash_font_t *titlefont;
	int dosnow;
};

void titlescreen(void);
void selectscreen(struct gamedata_s *gd);
void optionscreen(struct gamedata_s *gd);
void gameloop(struct gamedata_s *gd);
void doplayerphysics(bubble_sprite_t *sp, struct playerdata_s *pldat,
                     int plhandle, int plw, int plh);

int main(int argc, char **argv)
{
	if(air_init(SCRW, SCRH, AIR_8BPP) != 1) {
		exit(EXIT_FAILURE);
	}

	if(metal_init() != 1) {
		exit(EXIT_FAILURE);
	}

	titlescreen();

	metal_close();
	air_close();
	exit(EXIT_SUCCESS);
}

void titlescreen(void)
{
	struct gamedata_s gd;
	flash_image_t *titlebg;
	int menulen = 3;
	char *menu[] = { "Start", "Options", "Quit" };
	void *qh;
	int pos, i;

	heat_flatfadeout(1, 1);
	titlebg = flash_loadpcx("titlebg.pcx");
	heat_flatfadein(titlebg, FRAMESPERSECOND, FADETIME);
	gd.titlefont = crash_loadrawfont("slant.f14", 8, 14,
	                                 flash_closestcolor(255, 255, 255));
	gd.p1collide = rectangle;
	gd.p2collide = rectangle;
	gd.dosnow = 0;
	pos = 0;
	while(1) {
		metal_update();
		if(metal_ishit(METAL_K_ESCAPE))
			break;
		if(metal_ishit(METAL_K_ENTER)) {
			while(metal_ishit(METAL_K_ENTER)) metal_update();

			if(pos == 0) { /* start */
				selectscreen(&gd);
				heat_flatfadein(titlebg, FRAMESPERSECOND, FADETIME);
			} else if(pos == 1) { /* options */
				optionscreen(&gd);
			} else if(pos == 2) { /* exit */
				break;
			}
		}
		if(metal_ishit(METAL_K_DOWN)) {
			while(metal_ishit(METAL_K_DOWN)) metal_update();
			if(pos < 2) pos++;
		}
		if(metal_ishit(METAL_K_UP)) {
			while(metal_ishit(METAL_K_DOWN)) metal_update();
			if(pos > 0) pos--;
		}

		qh = quick_start(FRAMESPERSECOND);
		air_vanillablit(titlebg, 0, 0);
		for(i = 0; i < menulen; i++)
			crash_printf(30, 100+i*(gd.titlefont->height+1),
			             gd.titlefont, "%s", menu[i]);
		crash_printf(30-2-gd.titlefont->width,
		             100+pos*(gd.titlefont->height+1), gd.titlefont,
		             "\x10");
		air_update();
		quick_stop(qh);
	}

	crash_delete(gd.titlefont);
	return;
}

void optionscreen(struct gamedata_s *gd)
{
	void *qh, *sh;
	flash_image_t *bg;
	int pos;

	pos = 0;
	bg = flash_loadpcx("options.pcx");
	sh = heat_snowinit(32);
	while(1) {
		metal_update();
		if(metal_ishit(METAL_K_ESCAPE))
			break;
		if(metal_ishit(METAL_K_ENTER)) {
			while(metal_ishit(METAL_K_ENTER)) metal_update();

			if(pos == 0) { /* P1 collide */
				if(gd->p1collide == none)
					gd->p1collide = rectangle;
				else if(gd->p1collide == rectangle)
					gd->p1collide = pixel;
				else if(gd->p1collide == pixel)
					gd->p1collide = alphathreshold;
				else
					gd->p1collide = none;

			} else if(pos == 1) { /* P2 collide */
				if(gd->p2collide == none)
					gd->p2collide = rectangle;
				else if(gd->p2collide == rectangle)
					gd->p2collide = pixel;
				else if(gd->p2collide == pixel)
					gd->p2collide = alphathreshold;
				else
					gd->p2collide = none;

			} else if(pos == 2) { /* snow */
				gd->dosnow ^= 1;

			} else if(pos == 3) { /* exit */
				break;
			}
		}
		if(metal_ishit(METAL_K_DOWN)) {
			while(metal_ishit(METAL_K_DOWN)) metal_update();
			if(pos < 3) pos++;
		}
		if(metal_ishit(METAL_K_UP)) {
			while(metal_ishit(METAL_K_DOWN)) metal_update();
			if(pos > 0) pos--;
		}

		qh = quick_start(FRAMESPERSECOND);
		air_vanillablit(bg, 0, 0);
		heat_snowupdate(sh);
		crash_printf(30, 100+0*(gd->titlefont->height+1),
		             gd->titlefont, "Player 1 collide: %s",
		             (gd->p1collide == none)?"none":
		             (gd->p1collide == rectangle)?"rectangle":
		             (gd->p1collide == pixel)?"pixel":
		             "alphathreshold");
		crash_printf(30, 100+1*(gd->titlefont->height+1),
		             gd->titlefont, "Player 2 collide: %s",
		             (gd->p2collide == none)?"none":
		             (gd->p2collide == rectangle)?"rectangle":
		             (gd->p2collide == pixel)?"pixel":
		             "alphathreshold");
		crash_printf(30, 100+2*(gd->titlefont->height+1),
		             gd->titlefont, "In-stage snow: %s",
		             gd->dosnow?"on":"off");
		crash_printf(30, 100+3*(gd->titlefont->height+1),
		             gd->titlefont, "Back to title screen");
		crash_printf(30-2-gd->titlefont->width,
		             100+pos*(gd->titlefont->height+1), gd->titlefont,
		             "\x10");

		air_update();
		quick_stop(qh);
	}
	flash_imgdelete(bg);
	heat_snowclose(sh);
	return;
}

void selectscreen(struct gamedata_s *gd)
{
	void *qh;
	int chrlen = 2, i, pos;
	char buffer[81];
	char *chars[] = { "hao.spr", "ukyo.spr" };
	flash_image_t *selectbg, **selectchr, **selectchrsel;

	selectbg = flash_loadpcx("selectbg.pcx");
	heat_flatfadescrimg(selectbg, FRAMESPERSECOND, FADETIME);
	selectchr = malloc(chrlen*sizeof(flash_image_t *));
	for(i = 0; i < chrlen; i++) {
		sprintf(buffer, "sunchr%02d.pcx", i);
		selectchr[i] = flash_loadpcx(buffer);
	}
	selectchrsel = malloc(chrlen*sizeof(flash_image_t *));
	for(i = 0; i < chrlen; i++) {
		sprintf(buffer, "selchr%02d.pcx", i);
		selectchrsel[i] = flash_loadpcx(buffer);
	}

	pos = 0;
	while(1) {
		metal_update();
		if(metal_ishit(METAL_K_ESCAPE)) {
			while(metal_ishit(METAL_K_ESCAPE)) metal_update();
			break;
		}
		if(metal_ishit(METAL_K_LEFT)) {
//			while(metal_ishit(METAL_K_LEFT)) metal_update();
			if(pos > 0) pos--;
		}
		if(metal_ishit(METAL_K_RIGHT)) {
//			while(metal_ishit(METAL_K_RIGHT)) metal_update();
			if(pos < chrlen-1) pos++;
		}
		if(metal_ishit(METAL_K_ENTER)) {
			gd->pl1 = bubble_spriteload(chars[pos]);
			gd->pl2 = bubble_spriteload(chars[pos^1]);
			gameloop(gd);
			break;
		}

		qh = quick_start(FRAMESPERSECOND);
		air_vanillablit(selectbg, 0, 0);
		for(i = 0; i < chrlen; i++) {
			if(pos == i)
				air_blit(selectchrsel[i],
				         i*(selectchrsel[i]->width+1)+5, 140);
			else
				air_blit(selectchr[i],
				         i*(selectchr[i]->width+1)+5, 140);
		}
		air_update();
		quick_stop(qh);
	}

	flash_imgdelete(selectbg);
	for(i = 0; i < chrlen; i++) {
		flash_imgdelete(selectchr[i]);
		flash_imgdelete(selectchrsel[i]);
	}
	free(selectchr);
	free(selectchrsel);
	return;
}

void gameloop(struct gamedata_s *gd)
{
	void *qh, *sh;
	int pl1w, pl1h, pl2w, pl2h;
	flash_image_t *bg;

	bubble_init();
	wood_wipe(SCRW, SCRH, AIR_8BPP);
	bg = flash_loadpcx("stagebg.pcx");
	wood_addspritetobg(bg, 1);
	heat_flatfadescrimg(bg, FRAMESPERSECOND, FADETIME);
	gd->pl1handle = bubble_addsprite(gd->pl1, 0);
	gd->pl2handle = bubble_addsprite(gd->pl2, 0);

	pl1w = gd->pl1->anims[gd->pl1->curanim]->frames[gd->pl1->anims[gd->pl1->curanim]->curframe/gd->pl1->anims[gd->pl1->curanim]->framelag]->width;
	pl1h = gd->pl1->anims[gd->pl1->curanim]->frames[gd->pl1->anims[gd->pl1->curanim]->curframe/gd->pl1->anims[gd->pl1->curanim]->framelag]->height;
	gd->pl1->x = 20;
	gd->pl1->y = FLOORY-pl1h;
	gd->pl1sup.vx = 0;
	gd->pl1sup.vy = 0;
	gd->pl1sup.face = 0;
	gd->pl1sup.duck = 0;
	gd->pl1->collidemode = gd->p1collide;
	bubble_setspriteanim(gd->pl1handle, ANIMWALKFR);

	pl2w = gd->pl2->anims[gd->pl2->curanim]->frames[gd->pl2->anims[gd->pl2->curanim]->curframe/gd->pl2->anims[gd->pl2->curanim]->framelag]->width;
	pl2h = gd->pl2->anims[gd->pl2->curanim]->frames[gd->pl2->anims[gd->pl2->curanim]->curframe/gd->pl2->anims[gd->pl2->curanim]->framelag]->height;
	gd->pl2->x = SCRW-20-pl2w;
	gd->pl2->y = FLOORY-pl2h;
	gd->pl2sup.vx = 0;
	gd->pl2sup.vy = 0;
	gd->pl2sup.face = 1;
	gd->pl2sup.duck = 0;
	gd->pl2->collidemode = gd->p2collide;
	bubble_setspriteanim(gd->pl2handle, ANIMWALKFL);

	if(gd->dosnow) sh = heat_snowinit(128);

	while(1) {
	pl2w = gd->pl2->anims[gd->pl2->curanim]->frames[gd->pl2->anims[gd->pl2->curanim]->curframe/gd->pl2->anims[gd->pl2->curanim]->framelag]->width;
	pl2h = gd->pl2->anims[gd->pl2->curanim]->frames[gd->pl2->anims[gd->pl2->curanim]->curframe/gd->pl2->anims[gd->pl2->curanim]->framelag]->height;
	pl1w = gd->pl1->anims[gd->pl1->curanim]->frames[gd->pl1->anims[gd->pl1->curanim]->curframe/gd->pl1->anims[gd->pl1->curanim]->framelag]->width;
	pl1h = gd->pl1->anims[gd->pl1->curanim]->frames[gd->pl1->anims[gd->pl1->curanim]->curframe/gd->pl1->anims[gd->pl1->curanim]->framelag]->height;

		metal_update();
		gd->pl1->animate = 0;
		gd->pl2->animate = 0;
		if(metal_ishit(METAL_K_ESCAPE)) {
			while(metal_ishit(METAL_K_ESCAPE)) metal_update();
			break;
		}
		if(metal_ishit(METAL_K_LEFT) && gd->pl1->y > FLOORY-pl1h-1) {
			bubble_setspriteanim(gd->pl1handle,
			                     gd->pl1sup.duck?ANIMDUCKFL:ANIMWALKFL);
			gd->pl1->animate |= 1;
			gd->pl1sup.face = 1;
			if(gd->pl1sup.vx > -VXMAX)
				gd->pl1sup.vx -= VXSTEP;
		}
		if(metal_ishit(METAL_K_RIGHT) && gd->pl1->y > FLOORY-pl1h-1) {
			bubble_setspriteanim(gd->pl1handle,
			                     gd->pl1sup.duck?ANIMDUCKFR:ANIMWALKFR);
			gd->pl1->animate |= 1;
			gd->pl1sup.face = 0;
			if(gd->pl1sup.vx < VXMAX)
				gd->pl1sup.vx += VXSTEP;
		}
		if(metal_ishit(METAL_K_UP)) {
			bubble_setspriteanim(gd->pl1handle,
			                     gd->pl1sup.face?ANIMJUMPFL:ANIMJUMPFR);
			gd->pl1->animate |= 1;
			/* jump */
			if(gd->pl1->y > FLOORY-(pl1h+pl1h/2)) {
				if(gd->pl1sup.vy > -VYMAX)
					gd->pl1sup.vy -= VYSTEP;
				gd->pl1->y-=VYSTEP;
			}
		}
		if(metal_ishit(METAL_K_DOWN)) {
			/* duck */
			bubble_setspriteanim(gd->pl1handle,
			                     gd->pl1sup.face?ANIMDUCKFL:ANIMDUCKFR);
			gd->pl1sup.duck = 1;
		} else
			gd->pl1sup.duck = 0;
		if(metal_ishit(METAL_K_ENTER)) {
			/* pause */
		}

		qh = quick_start(FRAMESPERSECOND);

		if(bubble_checkspritespritecollide(gd->pl1, gd->pl2)) {
			if(gd->pl1->x < gd->pl2->x) {
				if(gd->pl1sup.vx > -gd->pl2sup.vx) {
					gd->pl2->x+=XPUSH;
				} else {
					gd->pl1->x-=XPUSH;
				}
			} else {
				if(gd->pl2sup.vx > -gd->pl1sup.vx) {
					gd->pl1->x+=XPUSH;
				} else {
					gd->pl2->x-=XPUSH;
				}
			}
		}

		doplayerphysics(gd->pl1, &gd->pl1sup, gd->pl1handle, pl1h,
		                pl1w);
		doplayerphysics(gd->pl2, &gd->pl2sup, gd->pl2handle, pl2h,
		                pl2w);

		/* camera tricks */
		while(gd->pl1->x-wood_camera_x < XPANPOS && wood_camera_x > 0)
			wood_pan(-1, 0);
		while(gd->pl1->x+pl1w-wood_camera_x > SCRW-XPANPOS &&
		      wood_camera_x+air_mode_w < wood_bound_w-1)
			wood_pan(1, 0);
		while(gd->pl1->y-wood_camera_y < YPANPOS && wood_camera_y > 0)
			wood_pan(0, -1);
		while(gd->pl1->y+pl1h-wood_camera_y > SCRH-YPANPOS &&
		      wood_camera_y+air_mode_h < wood_bound_h-1)
			wood_pan(0, 1);

		wood_updatebg();
		if(gd->dosnow) heat_snowupdate(sh);
		bubble_updatespr();
		wood_updatefg();

		air_update();
		quick_stop(qh);
	}

	if(gd->dosnow) heat_snowclose(sh);

	bubble_close();
	wood_wipe(0, 0, 0);
	return;
}

void doplayerphysics(bubble_sprite_t *sp, struct playerdata_s *pldat,
                     int plhandle, int plw, int plh)
{
	if(pldat->vy != 0 || sp->y+plh < FLOORY) {
		bubble_pansprite(plhandle, 0, pldat->vy);
		pldat->vy += VYACCEL;
	} else
		pldat->vy = 0;

	if(pldat->vx != 0) {
		if(sp->x >= 0 && sp->x+plw < wood_bound_w)
			bubble_pansprite(plhandle, pldat->vx, 0);
		if(pldat->vx < 0 && sp->y > FLOORY-plh-1) {
			pldat->vx += VXACCEL;
			if(pldat->vx > 0) pldat->vx = 0;
		} else if(sp->y > FLOORY-plh-1) {
			pldat->vx -= VXACCEL;
			if(pldat->vx < 0) pldat->vx = 0;
		}
	}

	if(sp->x < 0) { sp->x = 0; pldat->vx = 0; }
	if(sp->x+plw >= wood_bound_w) {
		sp->x = wood_bound_w-plw-1; pldat->vx = 0;
	}
	if(sp->y < 0) { sp->y = 0; pldat->vy = 0; }
	if(sp->y > FLOORY-plh) {
		sp->y = FLOORY-plh; pldat->vy = 0;
	        bubble_setspriteanim(plhandle,
		                     (pldat->face)?ANIMWALKFL:ANIMWALKFR);
	}
	return;
}

/* EOF test4.c */
