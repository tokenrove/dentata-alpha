/*
 * heat
 */

#ifndef HEAT_H
#define HEAT_H

#include <crash.h>

typedef enum { nil, flat, bevel, curvebevel } heat_boxstyle_t;

extern void heat_box(int, int, int, int, heat_boxstyle_t, int, int);
extern void heat_dialog(crash_font_t *, char *);
extern int heat_menu(crash_font_t *, int, char **);

extern void *heat_snowinit(int);
extern void heat_snowupdate(void *);
extern void heat_snowclose(void *);

extern void heat_flatfadein(flash_image_t *, int, int);
extern void heat_flatfadeout(int, int);
extern void heat_flatfadescrimg(flash_image_t *, int, int);

#endif

/* EOF heat.h */
