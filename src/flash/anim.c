/*
 * Animation functions for flash
 */

#include <flash.h>
#include <stdlib.h>

flash_anim_t *flash_animnew(int);
void flash_animdelete(flash_anim_t *);
flash_anim_t *flash_animdup(flash_anim_t *);
void flash_animaddframe(flash_anim_t *, flash_image_t *);
flash_image_t *flash_animcurframe(flash_anim_t *);
flash_image_t *flash_animnextframe(flash_anim_t *);

flash_anim_t *flash_animnew(int framelag)
{
	flash_anim_t *p;

	p = malloc(sizeof(flash_anim_t));
	if(p == NULL) return NULL;
	p->nframes = 0;
	p->frames = NULL;
	p->curframe = 0;
	p->framelag = framelag;

	return p;
}

void flash_animdelete(flash_anim_t *p)
{
	while(p->nframes-- > 0)
		free(p->frames[p->nframes]);
	free(p);
	return;
}

flash_anim_t *flash_animdup(flash_anim_t *p)
{
	flash_anim_t *q;

	q = flash_animnew(p->framelag);
	if(q == NULL) return NULL;
	q->nframes = p->nframes;
	q->frames = malloc(q->nframes*sizeof(flash_image_t *));
	q->curframe = 0;
	if(q->frames == NULL && q->nframes)
		return NULL;
	memcpy(q->frames, p->frames, q->nframes*sizeof(flash_image_t *));
	return q;
}

void flash_animaddframe(flash_anim_t *p, flash_image_t *i)
{
	p->nframes++;
	p->frames = realloc(p->frames, p->nframes*sizeof(flash_anim_t *));
	p->frames[p->nframes-1] = i;
	return;
}

flash_image_t *flash_animcurframe(flash_anim_t *p)
{
	return p->frames[p->curframe/p->framelag];
}

flash_image_t *flash_animnextframe(flash_anim_t *p)
{
	flash_image_t *i;

	i = p->frames[p->curframe/p->framelag];
	p->curframe++;
	p->curframe %= p->nframes*p->framelag;
	return i;
}

/* EOF anim.c */
