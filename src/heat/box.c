/*
 * Box/menu components of heat
 */

#include <heat.h>
#include <flash.h>
#include <air.h>

void heat_box(int, int, int, int, heat_boxstyle_t, int, int);

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

/* EOF box.c */
