/*
 * heat
 */

#ifndef HEAT_H
#define HEAT_H

#include <crash.h>

typedef enum { nil, flat, bevel, curvebevel } heat_boxstyle_t;

extern void heat_box(int, int, int, int, heat_boxstyle_t, int, int);
extern void heat_dialog(crash_font_t *, char *);

#endif

/* EOF heat.h */
