/*
 * bubble
 */

#ifndef BUBBLE_H
#define BUBBLE_H

#include <flash.h>

typedef enum { none, rectangle, pixel, alphathreshold } bubble_collidemode_t;

typedef struct bubble_rect_s {
	int x, y, w, h;
} bubble_rect_t;

typedef struct bubble_sprite_s {
	int x, y;
	bubble_collidemode_t collidemode;
	int curanim, nanims;
	flash_anim_t **anims;
	bubble_rect_t colliderect;
} bubble_sprite_t;

extern int bubble_init(void);
extern int bubble_addsprite(bubble_sprite_t *, int);
extern int bubble_delsprite(int);
extern int bubble_pansprite(int, int, int);
extern int bubble_jumpsprite(int, int, int);
extern void bubble_setspriteanim(int, int);
extern bubble_sprite_t *bubble_getspritebyhandle(int);
extern void bubble_updatespr(void);
extern void bubble_close(void);

extern bubble_sprite_t *bubble_spriteload(char *);
extern bubble_sprite_t *bubble_spritenew(void);
extern void bubble_spritedelete(bubble_sprite_t *);
extern bubble_sprite_t *bubble_spritedup(bubble_sprite_t *);
extern int bubble_spritegencolliderect(bubble_sprite_t *);
extern int bubble_checkspritespritecollide(bubble_sprite_t *,
                                           bubble_sprite_t *);
extern int bubble_checkspriterectcollide(bubble_sprite_t *, bubble_rect_t);

#endif

/* EOF bubble.h */
