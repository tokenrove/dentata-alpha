/*
 * metal
 */

#ifndef METAL_H
#define METAL_H

#define METAL_K_ESCAPE 1
#define METAL_K_UP     2
#define METAL_K_DOWN   3
#define METAL_K_LEFT   4
#define METAL_K_RIGHT  5
#define METAL_K_ENTER  6
#define METAL_K_LAST   7

extern int metal_init(void);
extern void metal_update(void);
extern int metal_ishit(int);
extern void metal_close(void);

#endif

/* EOF metal.h */
