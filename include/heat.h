/*
 * heat
 */

#ifndef HEAT_H
#define HEAT_H

typedef enum { nil, flat, bevel, curvebevel } heat_boxstyle_t;

extern void heat_box(int, int, int, int, heat_boxstyle_t, int, int);

#endif

/* EOF heat.h */
