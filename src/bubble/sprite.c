/*
 * Sprite components of bubble
 */

#include <bubble.h>
#include <flash.h>
#include <stdio.h>
#include <stdlib.h>

bubble_sprite_t *bubble_spriteload(char *);
bubble_sprite_t *bubble_spritenew(void);
void bubble_spritedelete(bubble_sprite_t *);
bubble_sprite_t *bubble_spritedup(bubble_sprite_t *);
int bubble_spritegencolliderect(bubble_sprite_t *);
int bubble_checkspritespritecollide(bubble_sprite_t *, bubble_sprite_t *);
int bubble_checkspriterectcollide(bubble_sprite_t *, bubble_rect_t);

bubble_sprite_t *bubble_spritenew(void)
{
	bubble_sprite_t *p;

	p = malloc(sizeof(bubble_sprite_t));
	if(p == NULL) return NULL;

	p->x = p->y = 0;
	p->collidemode = none;
	p->curanim = 0;
	p->nanims = 0;
	p->anims = NULL;
	return p;
}

void bubble_spritedelete(bubble_sprite_t *p)
{
	while(p->nanims-- > 0)
		flash_animdelete(p->anims[p->nanims]);
	free(p->anims);
	free(p);
}

bubble_sprite_t *bubble_spriteload(char *filename)
{
	FILE *fp;
	bubble_sprite_t *p;
	int i, j, form;
	char buffer[81], *q;

	fp = fopen(filename, "rb");
	if(fp == NULL) return NULL;

	p = bubble_spritenew();
	if(p == NULL) return NULL;

	p->x = fgetc(fp)|(fgetc(fp)<<8);
	p->y = fgetc(fp)|(fgetc(fp)<<8);
	p->collidemode = fgetc(fp);
	p->colliderect.x = fgetc(fp)|(fgetc(fp)<<8);
	p->colliderect.y = fgetc(fp)|(fgetc(fp)<<8);
	p->colliderect.w = fgetc(fp)|(fgetc(fp)<<8);
	p->colliderect.h = fgetc(fp)|(fgetc(fp)<<8);
	p->nanims = fgetc(fp)|(fgetc(fp)<<8);
	p->anims = malloc(p->nanims*sizeof(flash_anim_t*));
	if(p->anims == NULL && p->nanims != 0) return NULL;
	for(i = 0; i < p->nanims; i++) {
		p->anims[i] = flash_animnew(fgetc(fp)|(fgetc(fp)<<8));
		j = fgetc(fp)|(fgetc(fp)<<8);
		while(j-- > 0) {
			form = fgetc(fp);
			q = buffer;
			do { *(q++) = fgetc(fp); } while(*(q-1) != 0);
			if(form == 1) {
				flash_animaddframe(p->anims[i],
				                   flash_loadpcx(buffer));
			}
		}
	}

	return p;
}

bubble_sprite_t *bubble_spritedup(bubble_sprite_t *p)
{
	bubble_sprite_t *q;
	int i;

	q = bubble_spritenew();
	if(q == NULL) return NULL;
	q->nanims = p->nanims;
	q->anims = malloc(q->nanims*sizeof(flash_anim_t*));
	if(q->anims == NULL && q->nanims != 0) return NULL;
	q->x = p->x; q->y = p->y;
	q->collidemode = p->collidemode;
	q->colliderect = p->colliderect;
	for(i = 0; i < q->nanims; i++) {
		q->anims[i] = flash_animdup(p->anims[i]);
	}
	return q;
}

int bubble_checkspritespritecollide(bubble_sprite_t *a, bubble_sprite_t *b)
{
	int i, j;

	switch(a->collidemode) {
	case rectangle:
		switch(b->collidemode) {
		case rectangle:
			for(i = a->y+a->colliderect.y;
			    i < a->y+a->colliderect.y+a->colliderect.h;
			    i++) {
				for(j = a->x+a->colliderect.x;
				    j < a->x+a->colliderect.x+a->colliderect.w;
				    j++) {
					if(j >= b->x+b->colliderect.x &&
					   j <= b->x+b->colliderect.x+b->colliderect.w &&
					   i >= b->y+b->colliderect.y &&
					   i <= b->y+b->colliderect.y+b->colliderect.h)
						return 1;
				}
			}
			break;
		case pixel:
		case none:
		default:
			break;
		}
	case pixel:
	case none:
	default:
		break;
	}
	return 0;
}

int bubble_spritegencolliderect(bubble_sprite_t *p)
{
	bubble_rect_t r;
	int i, j;

	r.x = 0; r.y = 0; r.w = 0; r.h = 0;
	for(i = 0; i < p->nanims; i++)
		for(j = 0; j < p->anims[i]->nframes; j++) {
			if(p->anims[i]->frames[j]->width > r.w)
				r.w = p->anims[i]->frames[j]->width;
			if(p->anims[i]->frames[j]->height > r.h)
				r.h = p->anims[i]->frames[j]->height;
		}
	p->colliderect = r;
	return 1;
}

/* EOF sprite.c */
