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
void heat_dialog(crash_font_t *font, char *s);

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

void heat_dialog(crash_font_t *font, char *s)
{
	int x = 0, w, h, i, nlines;
	char *p, *q, buffer[81];
	void *qh;

	nlines = 1;
	w = 4+(font->width+1)*(strcspn(s, "\n")+1);
	q = s;
	while((q = strchr(q, '\n')) != NULL) {
		q++; nlines++;
		w = max(w, 4+(font->width+1)*(strcspn(q, "\n")+1));
	}
	h = 4+(font->height+1)*nlines;

	while(metal_ishit(METAL_K_ENTER)) metal_update();
	while(1) {
		metal_update();
		if(metal_ishit(METAL_K_ENTER))
			break;

		qh = quick_start(FRAMESPERSECOND);

		heat_box(80-font->width-1, 60-1, w, h, flat,
		         flash_closestcolor(255,255,255,air_getpalette()),
		         flash_closestcolor(0,0,255,air_getpalette()));
		q = p = s; i = 0;
		while((q = strchr(q, '\n')) != NULL) {
			q++;
			strncpy(buffer, p, strcspn(p, "\n")-1);
			crash_printf(80, 60+(font->height+1)*i+1, font, "%s", buffer);
			p = q;
			i++;
			if(i > nlines) break;
		}
		crash_printf(80, 60+(font->height+1)*i+1, font, "%s", p);
/*
		if((x/6)%2 == 0)
			crash_printf(100-gd.font->width,
			             40+1+(pos*gd.font->height),
		                     gd.font, ">");
		x++;
 */

		air_update();
		quick_stop(qh);
	}
	while(metal_ishit(METAL_K_ENTER)) metal_update();
	return;
}

/* EOF box.c */
