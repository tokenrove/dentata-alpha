/*
 * Box/menu components of heat
 */

#include <heat.h>
#include <flash.h>
#include <air.h>
#include <metal.h>
#include <crash.h>
#include <quick.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FRAMESPERSECOND 24

void heat_box(int, int, int, int, heat_boxstyle_t, int, int);
void heat_dialog(crash_font_t *, char *, int, int, int, int);
int heat_menu(crash_font_t *, int, char **);

void heat_box(int x, int y, int w, int h, heat_boxstyle_t style, int bcol,
              int fill)
{
	flash_image_t *p;
	char *q;
	int i;

	p = flash_imgnew(w, h, AIR_8BPP);
	q = p->data;
	memset(q, bcol, w); q += w;
	for(i = 0; i < h-2; i++) {
		*(q++) = bcol; memset(q, fill, w-2); q += w-2; *(q++) = bcol;
	}
	memset(q, bcol, w); q += w;
	air_blit(p, x, y);
	flash_imgdelete(p);
}

#define max(x, y) (((x) < (y)) ? (y) : (x))

void heat_dialog(crash_font_t *font, char *s, int x, int y, int w, int h)
{
	int counter = 0, i, pos, nlines, maxlines;
	char *p, *q, buffer[81];
	void *qh;

	nlines = 1;
	if(w == 0) counter = 1;
	if(counter == 1)
		w = 4+(font->width+1)*(strcspn(s, "\n")+1);
	q = s;
	while((q = strchr(q, '\n')) != NULL) {
		q++; nlines++;
		if(counter == 1)
			w = max(w, 4+(font->width+1)*(strcspn(q, "\n")+1));
	}
	if(h == 0)
		h = 4+(font->height+1)*nlines;
	maxlines = (h-4)/(font->height+1);
	pos = 0;

	while(metal_ishit(METAL_K_ENTER)) metal_update();
	while(1) {
		metal_update();
		if(metal_ishit(METAL_K_ENTER)) {
			while(metal_ishit(METAL_K_ENTER)) metal_update();
			if(pos+maxlines < nlines)
				pos += maxlines;
			else
				break;
		}

		qh = quick_start(FRAMESPERSECOND);

		heat_box(x, y, w, h, flat,
		         flash_closestcolor(255,255,255,air_getpalette()),
		         flash_closestcolor(0,0,255,air_getpalette()));
		q = s; i = 0;
		while(i < pos && q != NULL) {
			q = strchr(q, '\n');
			if(q != NULL) q++;
			i++;
		}
		i = 0; p = q;
		while(p != NULL && i < maxlines) {
			q = strchr(q, '\n');
			if(q != NULL) q++;
			memset(buffer, 0, 81);
			strncpy(buffer, p, strcspn(p, "\n"));
			crash_printf(x+2, y+(font->height+1)*i+2,
			             font, "%s", buffer);
			p = q;
			i++;
		}

		if(i+pos < nlines) {
			if((counter/6)%2 == 0)
				crash_printf(x+w-font->width,
				             y+h-font->height,
		        	             font, "\x10");
			counter++;
		}

		air_update();
		quick_stop(qh);
	}
	while(metal_ishit(METAL_K_ENTER)) metal_update();
	return;
}

int heat_menu(crash_font_t *font, int menulen, char **menu)
{
	int x = 0, w, h, i, pos = 0;
	void *qh;

	w = 0;
	for(i = 0; i < menulen; i++) {
		w = max(w, 4+(font->width+1)*(strlen(menu[i])+1));
	}
	h = 4+(font->height+1)*menulen;

	while(metal_ishit(METAL_K_ENTER)) metal_update();
	while(1) {
		metal_update();
		if(metal_ishit(METAL_K_DOWN))
			if(pos < menulen-1) pos++;
		if(metal_ishit(METAL_K_UP))
			if(pos > 0) pos--;
		if(metal_ishit(METAL_K_ENTER))
			break;
		if(metal_ishit(METAL_K_ESCAPE)) {
			pos = -1;
			break;
		}

		qh = quick_start(FRAMESPERSECOND);

		heat_box(100-font->width-1, 40-1, w, h, flat,
		         flash_closestcolor(255,255,255,air_getpalette()),
		         flash_closestcolor(0,0,255,air_getpalette()));
		for(i = 0; i < menulen; i++) {
			crash_printf(100, 40+(font->height+1)*i+1, font, "%s", menu[i]);
		}

		if((x/6)%2 == 0)
			crash_printf(100-font->width,
			             40+1+pos*(font->height+1),
		                     font, "\x10");
		x++;

		air_update();
		quick_stop(qh);
	}
	while(metal_ishit(METAL_K_ENTER) || metal_ishit(METAL_K_ESCAPE))
		metal_update();
	return pos;
}

/* EOF box.c */
