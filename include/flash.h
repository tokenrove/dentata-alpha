/*
 * flash
 * Image Handling Library
 * Julian Squires <tek@wiw.org> 2000
 */

#ifndef FLASH_H
#define FLASH_H

typedef struct flash_image_s {
	int width, height, type;
	int palettetype;

	char *data;
	char *alpha;
	unsigned char *palette;
} flash_image_t;

typedef struct flash_anim_s {
	int nframes, curframe, framelag;
	flash_image_t **frames;
} flash_anim_t;

extern flash_image_t *flash_loadpnm(char *);
extern flash_image_t *flash_loadpcx(char *);

extern void flash_loadpalette(char *, flash_image_t *);
extern int flash_closestcolor(int, int, int);
extern int flash_closestcolor16(int, int, int);
extern int flash_closestcolor8(int, int, int, unsigned char *);

extern flash_image_t *flash_imgnew(int, int, int);
extern void flash_imgdelete(flash_image_t *);
extern flash_image_t *flash_imgdup(flash_image_t *);
extern void flash_imgblit(flash_image_t *, flash_image_t *, int, int);

flash_image_t *flash_imgtypeconvert(flash_image_t *, int);

extern flash_anim_t *flash_animnew(int);
extern void flash_animdelete(flash_anim_t *);
extern flash_anim_t *flash_animdup(flash_anim_t *);
extern void flash_animaddframe(flash_anim_t *, flash_image_t *);
extern flash_image_t *flash_animcurframe(flash_anim_t *);
extern flash_image_t *flash_animnextframe(flash_anim_t *);

#endif

/* EOF flash.h */
