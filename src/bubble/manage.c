/*
 * Sprite management code for bubble
 */

#include <bubble.h>
#include <wood.h>
#include <air.h>
#include <stdlib.h>

int bubble_init(void);
int bubble_addsprite(bubble_sprite_t *, int);
int bubble_delsprite(int);
int bubble_pansprite(int, int, int);
int bubble_jumpsprite(int, int, int);
void bubble_setspriteanim(int, int);
bubble_sprite_t *bubble_getspritebyhandle(int);
void bubble_updatespr(void);
void bubble_close(void);

typedef struct bubble_stack_s {
	int nsprites;
	bubble_sprite_t **sprites;
} bubble_stack_t;

static bubble_stack_t sprstack;

int bubble_init(void)
{
	sprstack.nsprites = 0;
	sprstack.sprites = NULL;
	return 1;
}

void bubble_close(void)
{
	while(sprstack.nsprites-- > 0)
		if(sprstack.sprites[sprstack.nsprites])
			bubble_spritedelete(sprstack.sprites[sprstack.nsprites]);
	free(sprstack.sprites);
	return;
}

int bubble_addsprite(bubble_sprite_t *p, int priority)
{
	sprstack.nsprites++;
	sprstack.sprites = realloc(sprstack.sprites,
	                           sprstack.nsprites*sizeof(bubble_sprite_t*));
	sprstack.sprites[sprstack.nsprites-1] = p;
	return sprstack.nsprites-1;
}

int bubble_delsprite(int handle)
{
	bubble_spritedelete(sprstack.sprites[handle]);
	sprstack.sprites[handle] = NULL;
	return 1;
}

int bubble_pansprite(int handle, int x, int y)
{
	sprstack.sprites[handle]->x += x;
	sprstack.sprites[handle]->y += y;
	return 0;
}

int bubble_jumpsprite(int handle, int x, int y)
{
	sprstack.sprites[handle]->x = x;
	sprstack.sprites[handle]->y = y;
	return 0;
}

void bubble_setspriteanim(int handle, int anim)
{
	sprstack.sprites[handle]->curanim = anim;
	return;
}

bubble_sprite_t *bubble_getspritebyhandle(int handle)
{
	return sprstack.sprites[handle];
}

void bubble_updatespr(void)
{
	int i;
	bubble_sprite_t *p;

	for(i = 0; i < sprstack.nsprites; i++) {
		p = sprstack.sprites[i];
		if(p == NULL) continue;
		air_blit(flash_animnextframe(p->anims[p->curanim]),
		         p->x-wood_camera_x, p->y-wood_camera_y);
	}
	return;
}

/* EOF manage.c */
