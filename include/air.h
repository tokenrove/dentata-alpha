/*
 * air
 * A low-level graphics library.
 *
 * Julian Squires <tek@wiw.org> 2000
 */

#ifndef AIR_H
#define AIR_H

#define AIR_8BPP      0x1
#define AIR_16BPP     0x2
#define AIR_24BPP     0x4
#define AIR_GRAYSCALE 0x100
#define AIR_PALETTED  0x200
#define AIR_RGB       0x400
#define AIR_ALPHA0    0x1000
#define AIR_ALPHA1    0x2000
#define AIR_ALPHA8    0x4000

extern int air_mode_h, air_mode_w, air_mode_type;

extern int air_init(int, int, int);
extern void air_close(void);
extern void air_update(void);
extern void air_setpalette(unsigned char *);
extern unsigned char *air_getpalette(void);

#include <flash.h>

extern void air_blit(flash_image_t *, int, int);
extern void air_vanillablit(flash_image_t *, int, int);
extern unsigned char *air_grab(void);

#endif

/* EOF air.h */
