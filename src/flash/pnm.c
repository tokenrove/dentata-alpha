/*
 * PNM functions for flash
 */

#include <flash.h>
#include <air.h>
#include <stdio.h>
#include <stdlib.h>

#define PNMMAXLINE 70

flash_image_t *flash_loadpnm(char *);

flash_image_t *flash_loadpnm(char *filename)
{
	FILE *fp;
	flash_image_t *p;
	int type, w, h, inc;
	char buffer[PNMMAXLINE+1];

	fp = fopen(filename, "rb");
	if(fp == NULL) return NULL;

	buffer[0] = '\n';
	while(buffer[0] == '#' || buffer[0] == '\n')
		fgets(buffer, PNMMAXLINE, fp);
	if(buffer[0] == 'P') {
		if(buffer[1] == '5') /* PGM */
			type = AIR_8BPP;
		else if(buffer[1] == '6') /* PPM */
			type = AIR_24BPP;
		else /* unsupported */
			return NULL;
	} else
		return NULL;

	fgets(buffer, PNMMAXLINE, fp);
	sscanf(buffer, "%d %d", &w, &h);
	fgets(buffer, PNMMAXLINE, fp);
	/* max val ignored */

	p = flash_imgnew(w, h, type);
	if(p->type&AIR_8BPP) inc = 1;
	else if(p->type&AIR_16BPP) inc = 2;
	else if(p->type&AIR_24BPP) inc = 3;
	fread(p->data, w*h, inc, fp);

	fclose(fp);
	return p;
}

/* EOF pnm.c */
