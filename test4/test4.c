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
#define FRAMESPERSECOND 24
#define FLOORY 180
#define VXMAX 4
#define VXACCEL 1
#define VXSTEP 2
#define VYMAX 10
#define VYACCEL 1
#define VYSTEP 4
#define XPANPOS 10
#define YPANPOS 10

struct playerdata_s {
	int vx, vy;
};

struct gamedata_s {
	bubble_sprite_t *pl1;
	struct playerdata_s pl1sup;
	int pl1handle;
};

void titlescreen(void);
void selectscreen(struct gamedata_s *gd);
void gameloop(struct gamedata_s *gd);

int main(int argc, char **argv)
{
	printf("test4 v0.0 by tek [debugging output by dum's request]\n");
	printf("initializing air... "); fflush(stdout);
	if(air_init(SCRW, SCRH, AIR_8BPP) != 1) {
		printf("fail.\n");
		exit(EXIT_FAILURE);
	}
	printf("done.\n");

	printf("initializing metal... "); fflush(stdout);
	if(metal_init() != 1) {
		printf("fail.\n");
		exit(EXIT_FAILURE);
	}
	printf("done.\n");

	printf("entering title screen... "); fflush(stdout);
	titlescreen();
	printf("done.\n");

	printf("shutting down metal... "); fflush(stdout);
	metal_close();
	printf("done.\n");

	printf("shutting down air... "); fflush(stdout);
	air_close();
	printf("done.\n");
	exit(EXIT_SUCCESS);
}

void titlescreen(void)
{
	struct gamedata_s gd;
	flash_image_t *titlebg, *titlestart, *titlestartsel,
	              *titlequit, *titlequitsel;
	void *qh;
	int pos;

	printf("\n\twiping wood [initial]... "); fflush(stdout);
	wood_wipe(SCRW, SCRH, AIR_8BPP);
	printf("done.\n");

	printf("\ttitlescreen data load"); fflush(stdout);
	titlebg = flash_loadpcx("titlebg.pcx");
	printf("."); fflush(stdout);
	titlestart = flash_loadpcx("titlest0.pcx");
	printf("."); fflush(stdout);
	titlestartsel = flash_loadpcx("titlest1.pcx");
	printf("."); fflush(stdout);
	titlequit = flash_loadpcx("titleqt0.pcx");
	printf("."); fflush(stdout);
	titlequitsel = flash_loadpcx("titleqt1.pcx");
	printf("."); fflush(stdout);
	printf(" done.\n");

	printf("\ttitlescreen loop... "); fflush(stdout);
	air_setpalette(titlebg->palette);
	pos = 0;
	while(1) {
		metal_update();
		if(metal_ishit(METAL_K_ESCAPE))
			break;
		if(metal_ishit(METAL_K_ENTER)) {
			while(metal_ishit(METAL_K_ENTER)) metal_update();

			if(pos == 0) { /* start */
				selectscreen(&gd);
			} else if(pos == 1) { /* exit */
				break;
			}
		}
		if(metal_ishit(METAL_K_DOWN)) {
			if(pos < 1) pos++;
		}
		if(metal_ishit(METAL_K_UP)) {
			if(pos > 0) pos--;
		}

		qh = quick_start(FRAMESPERSECOND);
		air_vanillablit(titlebg, 0, 0);
		if(pos != 0)
			air_blit(titlestart, 20, 130);
		else
			air_blit(titlestartsel, 20, 130);
		if(pos != 1)
			air_blit(titlequit, 20, 130+titlestart->height+5);
		else
			air_blit(titlequitsel, 20, 130+titlestart->height+5);
		air_update();
		quick_stop(qh);
	}
	printf("done.\n");

	printf("\n\twiping wood [final]... "); fflush(stdout);
	wood_wipe(0, 0, 0);
	printf("done.\n");
	return;
}

void selectscreen(struct gamedata_s *gd)
{
	void *qh;
	int chrlen = 2, i, pos;
	char buffer[81];
	char *chars[] = { "hao.spr", "megaman.spr" };
	flash_image_t *selectbg, **selectchr, **selectchrsel;

	printf("\n\t\tentering select screen... "); fflush(stdout);

	printf("\n\t\tloading image data"); fflush(stdout);
	selectbg = flash_loadpcx("selectbg.pcx");
	printf("."); fflush(stdout);
	selectchr = malloc(chrlen*sizeof(flash_image_t *));
	for(i = 0; i < chrlen; i++) {
		sprintf(buffer, "sunchr%02d.pcx", i);
		selectchr[i] = flash_loadpcx(buffer);
		printf("."); fflush(stdout);
	}
	selectchrsel = malloc(chrlen*sizeof(flash_image_t *));
	for(i = 0; i < chrlen; i++) {
		sprintf(buffer, "selchr%02d.pcx", i);
		selectchrsel[i] = flash_loadpcx(buffer);
		printf("."); fflush(stdout);
	}

	printf("\n\t\tentering select loop... "); fflush(stdout);
	pos = 0;
	while(1) {
		metal_update();
		if(metal_ishit(METAL_K_ESCAPE)) {
			while(metal_ishit(METAL_K_ESCAPE)) metal_update();
			break;
		}
		if(metal_ishit(METAL_K_LEFT)) {
			while(metal_ishit(METAL_K_LEFT)) metal_update();
			if(pos > 0) pos--;
		}
		if(metal_ishit(METAL_K_RIGHT)) {
			while(metal_ishit(METAL_K_RIGHT)) metal_update();
			if(pos < chrlen-1) pos++;
		}
		if(metal_ishit(METAL_K_ENTER)) {
			printf("warning: should have deleted img data!\n");
			gd->pl1 = bubble_spriteload(chars[pos]);
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
	printf("\n\t\tdone.\n");

	flash_imgdelete(selectbg);
	for(i = 0; i < chrlen; i++) {
		flash_imgdelete(selectchr[i]);
		flash_imgdelete(selectchrsel[i]);
	}
	free(selectchr);
	free(selectchrsel);
	printf("\n\t\tdone.\n");
	return;
}

void gameloop(struct gamedata_s *gd)
{
	void *qh;
	int pl1w, pl1h;

	printf("\n\t\t\tentering gameloop... "); fflush(stdout);

	printf("\n\t\t\tloading data"); fflush(stdout);
	bubble_init();
	wood_wipe(SCRW, SCRH, AIR_8BPP);
	wood_addspritetobg(flash_loadpcx("stagebg.pcx"), 1);
	printf("."); fflush(stdout);
	gd->pl1handle = bubble_addsprite(gd->pl1, 0);
	printf("."); fflush(stdout);
	printf("done.\n");

	printf("\n\t\t\tlooping... "); fflush(stdout);
	gd->pl1->x = 20;
	pl1w = gd->pl1->anims[gd->pl1->curanim]->frames[gd->pl1->anims[gd->pl1->curanim]->curframe/gd->pl1->anims[gd->pl1->curanim]->framelag]->width;
	pl1h = gd->pl1->anims[gd->pl1->curanim]->frames[gd->pl1->anims[gd->pl1->curanim]->curframe/gd->pl1->anims[gd->pl1->curanim]->framelag]->height;
	gd->pl1->y = FLOORY-pl1h;
	gd->pl1sup.vx = 0;
	gd->pl1sup.vy = 0;
	while(1) {
		metal_update();
		gd->pl1->animate = 0;
		if(metal_ishit(METAL_K_ESCAPE)) {
			while(metal_ishit(METAL_K_ESCAPE)) metal_update();
			break;
		}
		if(metal_ishit(METAL_K_LEFT) && gd->pl1->y > FLOORY-pl1h-1) {
			gd->pl1->animate |= 1;
			if(gd->pl1sup.vx > -VXMAX)
				gd->pl1sup.vx -= VXSTEP;
		}
		if(metal_ishit(METAL_K_RIGHT) && gd->pl1->y > FLOORY-pl1h-1) {
			gd->pl1->animate = 1;
			if(gd->pl1sup.vx < VXMAX)
				gd->pl1sup.vx += VXSTEP;
		}
		if(metal_ishit(METAL_K_UP)) {
			/* jump */
			if(gd->pl1->y > FLOORY-(pl1h+pl1h/2)) {
				if(gd->pl1sup.vy > -VYMAX)
					gd->pl1sup.vy -= VYSTEP;
				gd->pl1->y-=VYSTEP;
			}
		}
		if(metal_ishit(METAL_K_DOWN)) {
			/* duck */
		}
		if(metal_ishit(METAL_K_ENTER)) {
			/* pause */
		}

		qh = quick_start(FRAMESPERSECOND);

		if(gd->pl1sup.vy != 0 || gd->pl1->y+pl1h < FLOORY) {
			bubble_pansprite(gd->pl1handle, 0, gd->pl1sup.vy);
			gd->pl1sup.vy += VYACCEL;
		} else
			gd->pl1sup.vy = 0;

		if(gd->pl1sup.vx != 0) {
			if(gd->pl1->x >= 0 && gd->pl1->x+pl1w < wood_bound_w)
				bubble_pansprite(gd->pl1handle, gd->pl1sup.vx, 0);
			if(gd->pl1sup.vx < 0 && gd->pl1->y > FLOORY-pl1h-1) {
				gd->pl1sup.vx += VXACCEL;
				if(gd->pl1sup.vx > 0) gd->pl1sup.vx = 0;
			} else if(gd->pl1->y > FLOORY-pl1h-1) {
				gd->pl1sup.vx -= VXACCEL;
				if(gd->pl1sup.vx < 0) gd->pl1sup.vx = 0;
			}
		}

		if(gd->pl1->x < 0) { gd->pl1->x = 0; gd->pl1sup.vx = 0; }
		if(gd->pl1->x+pl1w >= wood_bound_w) { gd->pl1->x = wood_bound_w-pl1w-1; }
		if(gd->pl1->y < 0) { gd->pl1->y = 0; gd->pl1sup.vy = 0; }
		if(gd->pl1->y-wood_camera_y+pl1h > FLOORY) { gd->pl1->y = FLOORY-pl1h; gd->pl1sup.vy = 0; }

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
		bubble_updatespr();
		wood_updatefg();

		air_update();
		quick_stop(qh);
	}
	printf("done.\n");

	bubble_close();
	wood_wipe(0, 0, 0);
	printf("\t\t\tdone.\n");
	return;
}

/* EOF test4.c */
