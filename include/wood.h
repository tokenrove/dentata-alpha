/*
 * wood
 */

#ifndef WOOD_H
#define WOOD_H

#include <flash.h>

typedef struct wood_tilemap_s {
	int width, height;
	char walkthresh;
	char *map;
	int tilew, tileh, tiletype, ntiles;
	flash_image_t **tiles;
} wood_tilemap_t;

extern int wood_camera_x, wood_camera_y;
extern int wood_bound_w, wood_bound_h;

extern int wood_pan(int, int);
extern void wood_jump(int, int);
extern void wood_wipe(int, int, int);
extern void wood_addspritetobg(flash_image_t *, int);
extern void wood_addanimtobg(flash_anim_t *, int);
extern void wood_addtilemaptobg(wood_tilemap_t *, int);
extern void wood_addspritetofg(flash_image_t *, int);
extern void wood_addtilemaptofg(wood_tilemap_t *, int);
extern void wood_updatebg(void);
extern void wood_updatefg(void);
extern wood_tilemap_t *wood_loadtilemap(char *);
extern void wood_delete(wood_tilemap_t *);
extern char wood_tiletype(int, int, int);

#endif

/* EOF wood.h */
