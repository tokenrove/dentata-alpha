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
#include <string.h>

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
void credits(struct gamedat_s gd);
void mainloop(struct gamedat_s gd);

void mainloop(struct gamedat_s gd)
{
	int i, nignore = 1, ignore[1];
	float theta;
	void *qh;
	int menulen[] = { 3, 2 };
	char ***menu;
	bubble_sprite_t *hp;

	menu = malloc(2*sizeof(char **));
	menu[0] = malloc(menulen[0]*sizeof(char *));
	menu[0][0] = strdup("Talk");
	menu[0][1] = strdup("Examine");
	menu[0][2] = strdup("Other \x10");
	menu[1] = malloc(menulen[1]*sizeof(char *));
	menu[1][0] = strdup("Status");
	menu[1][1] = strdup("Quit");

	ignore[0] = gd.hero;

	hp = bubble_getspritebyhandle(gd.head);
	bubble_spritegencolliderect(hp);
	hp = bubble_getspritebyhandle(gd.hero);

	while(1) {
		metal_update();
		if(metal_ishit(METAL_K_ESCAPE))
			break;
		if(metal_ishit(METAL_K_LEFT)) {
//			if(wood_iswalkablerect(hp->x-1, hp->y,
//			                       hp->colliderect.w,
//			                       hp->colliderect.h)){
			if(wood_tiletype(hp->x-1, hp->y, 0) < 2 &&
			   wood_tiletype(hp->x-1, hp->y+hp->colliderect.h-1, 0) < 2) {
				bubble_pansprite(gd.hero, -1, 0);
				if(hp->x-wood_camera_x < SCRW/2)
					wood_pan(-1, 0);
			}
			bubble_setspriteanim(gd.hero, 2);
			theta = M_PI;
		}
		if(metal_ishit(METAL_K_RIGHT)) {
//			if(wood_iswalkablerect(hp->x+1, hp->y,
//			                       hp->colliderect.w,
//			                       hp->colliderect.h)){
			if(wood_tiletype(hp->x+hp->colliderect.w, hp->y, 0) < 2 &&
			   wood_tiletype(hp->x+hp->colliderect.w, hp->y+hp->colliderect.h-1, 0) < 2) {
				bubble_pansprite(gd.hero, 1, 0);
				if(hp->x-wood_camera_x > SCRW/2)
					wood_pan(1, 0);
			}
			bubble_setspriteanim(gd.hero, 3);
			theta = 0;
		}
		if(metal_ishit(METAL_K_UP)) {
//			if(wood_iswalkablerect(hp->x, hp->y-1,
//			                       hp->colliderect.w,
//			                       hp->colliderect.h)){
			if(wood_tiletype(hp->x, hp->y-1, 0) < 2 &&
			   wood_tiletype(hp->x+hp->colliderect.w-1, hp->y-1, 0) < 2) {
				bubble_pansprite(gd.hero, 0, -1);
				if(hp->y-wood_camera_y < SCRH/2)
					wood_pan(0, -1);
			}
			bubble_setspriteanim(gd.hero, 0);
			theta = M_PI/2;
		}
		if(metal_ishit(METAL_K_DOWN)) {
//			if(wood_iswalkablerect(hp->x, hp->y+1,
//			                       hp->colliderect.w,
//			                       hp->colliderect.h)){
			if(wood_tiletype(hp->x, hp->y+hp->colliderect.h, 0) < 2 &&
			   wood_tiletype(hp->x+hp->colliderect.w-1, hp->y+hp->colliderect.h, 0) < 2) {
				bubble_pansprite(gd.hero, 0, 1);
				if(hp->y-wood_camera_y > SCRH/2)
					wood_pan(0, 1);
			}
			bubble_setspriteanim(gd.hero, 1);
			theta = (3*M_PI)/2;
		}
		if(metal_ishit(METAL_K_ENTER)) {
			i = heat_menu(gd.font, menulen[0], menu[0]);
			if(i == 0) { /* talk to */
				i = bubble_checkraycollide(hp->x, hp->y,
				                           theta,
				                           hp->colliderect.w*2,
				                           nignore, ignore);
				if(i == -1) {
					heat_dialog(gd.font,
					            "Who are you talking to?",
					            10, 10, 0, 4*(gd.font->height+1)+4);
				} else if(i == gd.head) {
					heat_dialog(gd.font,
					            "[HEAD]\nsm4sh th3 m1dg"
					            "3tz, r0d3nt!@#!@",
					            10, 10, 0, 4*(gd.font->height+1)+4);
				}
			} else if(i == 1) { /* examine */
				heat_dialog(gd.font, "The Dr Pepper does nothing.", 10, 10, 0, 4*(gd.font->height+1)+4);
			} else if(i == 2) { /* other */
				i = heat_menu(gd.font, menulen[1], menu[1]);
				if(i == 0) /* status */
					heat_dialog(gd.font,
                                                    "Don't worry about such \n"
                                                    "things. This isn't a game!\n"
					            "\n"
					            "This is, however, a test\n"
					            "of multiline dialog.", 10, 10, 0, 4*(gd.font->height+1)+4);
				else if(i == 1) /* quit */
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
	                            flash_closestcolor(255, 255, 255));

	wood_wipe(SCRW, SCRH, AIR_8BPP);
	wood_addtilemaptobg(wood_loadtilemap("world00.map"), 1);
	bubble_init();
	gd.hero = bubble_addsprite(bubble_spriteload("hero.spr"), 1);
	gd.head = bubble_addsprite(bubble_spriteload("head.spr"), 2);

	mainloop(gd);

	credits(gd);

	bubble_close();
	wood_wipe(0, 0, 0);
	crash_delete(gd.font);
	metal_close();
	air_close();
	exit(EXIT_SUCCESS);
}

void credits(struct gamedat_s gd)
{
	void *qh, *sh;
	int pos = SCRH;
	int msglen = 23;
	char *msg[] = { "Programming", "Tek", "",
                        "Concept", "Square and a thousand others", "",
	                "NES Graphics", "Obliviax", "",
	                "Thanks", "Retsyn", "Astfgl",
	                "Dum2007", "unSlider", "Taliesin",
	                "Fract", "Adoom", "",

	                "", "", "No floating samurai heads were",
	                "harmed in the production of ", "this demo" };
	int msgemph[] = { 1, 0, 0,
	                  1, 0, 0,
	                  1, 0, 0,
	                  1, 0, 0,  0, 0, 0,  0, 0, 0,

	                  0, 0, 0, 0, 0 };
	flash_image_t *bg;
	int i, j, inc, bginc;
	crash_font_t *emph;

	bg = flash_loadpcx("stars.pcx");
	heat_flatfadein(bg, FRAMESPERSECOND, 1000000);
	emph = crash_loadrawfont("future.f14", 8, 14,
	                         flash_closestcolor(192, 0, 0));
	sh = heat_snowinit(128);

	inc = bginc = 0;
	while(pos+inc > SCRH/2) {
		metal_update();
		if(metal_ishit(METAL_K_ESCAPE))
			break;

		qh = quick_start(24);

		air_vanillablit(bg, 0, bginc);

		for(i = 0, inc = 0; i < msglen; i++) {
			if(msgemph[i]) {
				crash_printf(SCRW/2-(strlen(msg[i])*(gd.font->width+1))/2,
				             pos+inc,
				             emph, msg[i]);
				inc += emph->height+1;
			} else {
				crash_printf(SCRW/2-(strlen(msg[i])*(gd.font->width+1))/2,
				             pos+inc,
				             gd.font, msg[i]);
				inc += gd.font->height+1;
			}
		}
		pos--;

		if(bginc > -99) bginc-=2;
		else heat_snowupdate(sh);

		air_update();
		quick_stop(qh);
	}

	for(j = 0; j < 1000000; j += 41666) {
		metal_update();
		if(metal_ishit(METAL_K_ESCAPE))
			break;

		qh = quick_start(24);
		air_vanillablit(bg, 0, bginc);

		for(i = 0, inc = 0; i < msglen; i++) {
			if(msgemph[i]) {
				crash_printf(SCRW/2-(strlen(msg[i])*(gd.font->width+1))/2,
				             pos+inc,
				             emph, msg[i]);
				inc += emph->height+1;
			} else {
				crash_printf(SCRW/2-(strlen(msg[i])*(gd.font->width+1))/2,
				             pos+inc,
				             gd.font, msg[i]);
				inc += gd.font->height+1;
			}
		}
		heat_snowupdate(sh);
		air_update();
		quick_stop(qh);
	}

	heat_snowclose(sh);

	flash_imgdelete(bg);
	heat_flatfadeout(FRAMESPERSECOND, 5000000);

	return;
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
