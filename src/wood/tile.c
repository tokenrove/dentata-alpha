/*
 * Tiling components for wood
 */

#include <wood.h>
#include <flash.h>

#include <stdio.h>
#include <stdlib.h>

wood_tilemap_t *wood_loadtilemap(char *filename)
{
	FILE *fp;
	wood_tilemap_t *p;
	char *q, buffer[81];
	char form;
	int i;

	p = malloc(sizeof(wood_tilemap_t));
	if(p == NULL) return NULL;

	fp = fopen(filename, "rb");
	if(fp == NULL) return NULL;

	p->width = fgetc(fp)|(fgetc(fp)<<8);
	p->height = fgetc(fp)|(fgetc(fp)<<8);
	p->walkthresh = fgetc(fp);
	p->ntiles = fgetc(fp);
	p->tiles = malloc(sizeof(flash_image_t *)*p->ntiles);
	if(p->tiles == NULL) return NULL;
	for(i = 0; i < p->ntiles; i++) {
		form = fgetc(fp);
		q = buffer;
		do { *(q++) = fgetc(fp); } while(*(q-1) != 0);
		if(form == 0) {
			p->tiles[i] = flash_loadpnm(buffer);
		} else if(form == 1) {
			p->tiles[i] = flash_loadpcx(buffer);
		}
		if(p->tiles[i] == NULL) return NULL;
	}
	p->map = malloc(p->width*p->height);
	if(p->map == NULL) return NULL;
	fread(p->map, p->width*p->height, 1, fp);

	p->tilew = p->tiles[0]->width;
	p->tileh = p->tiles[0]->height;
	p->tiletype = p->tiles[0]->type;

	fclose(fp);
	return p;
}

void wood_delete(wood_tilemap_t *p)
{
	int i;

	free(p->map);
	for(i = 0; i < p->ntiles; i++)
		flash_imgdelete(p->tiles[i]);
	free(p->tiles);
	return;
}

/* EOF tile.c */
