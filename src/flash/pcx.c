/*
 * PCX routines for flash
 */

#include <flash.h>
#include <air.h>

#include <stdio.h>
#include <stdlib.h>

flash_image_t *flash_loadpcx(char *);

flash_image_t *flash_loadpcx(char *filename)
{
	flash_image_t *p;
	FILE *fp;
	int type, i, run;
	short w, h;
	unsigned char c;

	fp = fopen(filename, "rb");
	if(fp == NULL) return NULL;

	fgetc(fp); /* manufacturer == 0 */
	if(fgetc(fp) != 5) return NULL; /* we only deal with 8bpp atm */
	type = AIR_8BPP|AIR_PALETTED;
	if(fgetc(fp) != 1) return NULL; /* encoding == 1 */
	if(fgetc(fp) != 8) return NULL; /* 8bpp, again */
	fseek(fp, 4, SEEK_CUR);
	fread(&w, 2, 1, fp); w++;
	fread(&h, 2, 1, fp); h++;
	fseek(fp, 116, SEEK_CUR);

	p = flash_imgnew(w, h, type);
	if(p == NULL) return NULL;
	p->palette = malloc(256*3);
	if(p->palette == NULL) return NULL;

	run = 0;
	for(i = 0; i < w*h;) {
		c = fgetc(fp);
		if(run == 0 && (c>0xBF)) { /* run */
			run = c&(~0xC0);
		} else {
			if(run == 0)
				p->data[i++] = c;
			for(; run; run--)
				if(i < w*h)
					p->data[i++] = c;
				else
					/* error condition */;
		}
	}

	fseek(fp, -768L, SEEK_END);
	fread(p->palette, 3, 256, fp);

	fclose(fp);
	return p;
}

/* EOF pcx.c */
