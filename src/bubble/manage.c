/*
 * Sprite management code for bubble
 */

#include <bubble.h>
#include <wood.h>
#include <air.h>
#include <stdlib.h>
#include <math.h>

int bubble_init(void);
int bubble_addsprite(bubble_sprite_t *, int);
int bubble_delsprite(int);
int bubble_pansprite(int, int, int);
int bubble_jumpsprite(int, int, int);
void bubble_setspriteanim(int, int);
bubble_sprite_t *bubble_getspritebyhandle(int);
void bubble_updatespr(void);
void bubble_close(void);
int bubble_checkraycollide(int, int, float, int, int, int *);

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

int bubble_checkraycollide(int x, int y, float theta, int radius,
	                   int nignore, int *ignore)
{
	float dx = 0, dy = 0;
	int i, j;

	while(radius-- > 0) {
		dx += cos(theta);
		if(dx > 1) { x++; dx -= 1; }
		else if(dx < -1) { x--; dx += 1; }
		dy += sin(theta);
		if(dy > 1) { y++; dy -= 1; }
		else if(dy < -1) { y--; dy += 1; }

		for(i = 0; i < sprstack.nsprites; i++) {
			for(j = 0; j < nignore; j++)
				if(i == ignore[j])
					break;
			if(j < nignore) continue;

			if(x >= sprstack.sprites[i]->x &&
			   x <= sprstack.sprites[i]->x+sprstack.sprites[i]->colliderect.w &&
			   y >= sprstack.sprites[i]->y &&
			   y <= sprstack.sprites[i]->y+sprstack.sprites[i]->colliderect.h)
				return i;
		}
	}

	return -1;
}

/* EOF manage.c */
