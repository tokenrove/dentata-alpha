/*
 * air
 * A low-level graphics library.
 *
 * Julian Squires <tek@wiw.org> 2000
 */

#ifndef AIR_H
#define AIR_H

#define AIR_8BPP 1
#define AIR_16BPP 2
#define AIR_24BPP 4

extern int air_init(int, int, int);
extern void air_close(void);
extern void air_update(void);
extern void air_setpalette(char *);
extern char *air_getpalette(void);

#include <flash.h>

extern void air_blit(flash_image_t *, int, int);
extern void air_vanillablit(flash_image_t *, int, int);

#endif

/* EOF air.h */
