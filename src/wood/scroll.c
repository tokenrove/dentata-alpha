/*
 * Scrolling components of wood
 */

#include <wood.h>
#include <flash.h>
#include <air.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct wood_stack_s {
	int nlayers;
	flash_image_t **layersim;
	flash_anim_t **layersam;
	wood_tilemap_t **layerstm;
	int *depths;
} wood_stack_t;

int wood_camera_x, wood_camera_y;
static int wood_w, wood_h, wood_type, wood_bound_w, wood_bound_h;
static wood_stack_t bgstack = { 0, NULL, NULL, NULL, NULL };
static wood_stack_t fgstack = { 0, NULL, NULL, NULL, NULL };

int wood_pan(int, int);
void wood_jump(int, int);
void wood_wipe(int, int, int);
void wood_addspritetobg(flash_image_t *, int);
void wood_addanimtobg(flash_anim_t *, int);
void wood_addtilemaptobg(wood_tilemap_t *, int);
void wood_addspritetofg(flash_image_t *, int);
void wood_addtilemaptofg(wood_tilemap_t *, int);
void wood_updatebg(void);
void wood_updatefg(void);

int wood_pan(int x, int y)
{
	int ret = 0;

	if(wood_camera_x+x >= 0 && wood_camera_x+x+wood_w < wood_bound_w)
		wood_camera_x += x;
	else ret |= 1;

	if(wood_camera_y+y >= 0 && wood_camera_y+y+wood_h < wood_bound_h)
		wood_camera_y += y;
	else ret |= 2;

	return ret;
}

void wood_jump(int x, int y)
{
	wood_camera_x = x;
	wood_camera_y = y;
	return;
}

void wood_wipe(int w, int h, int type)
{
	wood_w = w; wood_h = h; wood_type = type;
	wood_camera_x = 0; wood_camera_y = 0;
	wood_bound_w = 0; wood_bound_h = 0;
	while(bgstack.nlayers-- > 0) { 
		if(bgstack.layersim[bgstack.nlayers])
			flash_imgdelete(bgstack.layersim[bgstack.nlayers]);
		if(bgstack.layersam[bgstack.nlayers])
			flash_animdelete(bgstack.layersam[bgstack.nlayers]);
		if(bgstack.layerstm[bgstack.nlayers])
			wood_delete(bgstack.layerstm[bgstack.nlayers]);
	}
	bgstack.nlayers = 0;
	free(bgstack.layersim);
	free(bgstack.layersam);
	free(bgstack.layerstm);
	free(bgstack.depths);
	bgstack.layersim = NULL;
	bgstack.layersam = NULL;
	bgstack.layerstm = NULL;
	bgstack.depths = NULL;

	while(fgstack.nlayers-- > 0) { 
		if(fgstack.layersim[fgstack.nlayers])
			flash_imgdelete(fgstack.layersim[fgstack.nlayers]);
		if(fgstack.layersam[fgstack.nlayers])
			flash_animdelete(fgstack.layersam[fgstack.nlayers]);
		if(fgstack.layerstm[fgstack.nlayers])
			wood_delete(fgstack.layerstm[fgstack.nlayers]);
	}
	fgstack.nlayers = 0;
	free(fgstack.layersim);
	free(fgstack.layersam);
	free(fgstack.layerstm);
	free(fgstack.depths);
	fgstack.layersim = NULL;
	fgstack.layersam = NULL;
	fgstack.layerstm = NULL;
	fgstack.depths = NULL;
	return;
}

int wood_addlayer(wood_stack_t *stack, int depth)
{
	int i;

	stack->nlayers++;
	stack->depths = realloc(stack->depths, stack->nlayers*sizeof(int));
	stack->layersim = realloc(stack->layersim,
	                          stack->nlayers*sizeof(flash_image_t *));
	stack->layersam = realloc(stack->layersam,
	                          stack->nlayers*sizeof(flash_anim_t *));
	stack->layerstm = realloc(stack->layerstm,
	                          stack->nlayers*sizeof(wood_tilemap_t *));
	for(i = 0; i < stack->nlayers-1; i++) {
		if(stack->depths[i] < depth) {
			memmove(&stack->depths[i+1], &stack->depths[i],
				(stack->nlayers-1-i)*sizeof(int));
			memmove(&stack->layersam[i+1], &stack->layersam[i],
				(stack->nlayers-1-i)*sizeof(flash_anim_t*));
			memmove(&stack->layerstm[i+1], &stack->layerstm[i],
				(stack->nlayers-1-i)*sizeof(wood_tilemap_t*));
			break;
		}
	}

	return i;
}

#define min(x, y) (((x) > (y)) ? (y) : (x))

void wood_addspritetobg(flash_image_t *p, int depth)
{
	int i;

	i = wood_addlayer(&bgstack, depth);
	bgstack.layersim[i] = p;
	bgstack.layerstm[i] = NULL;
	bgstack.depths[i] = depth;

	if(wood_bound_w == 0)
		wood_bound_w = p->width*depth;
	else
		wood_bound_w = min(wood_bound_w, p->width*depth);
	if(wood_bound_h == 0)
		wood_bound_h = p->height*depth;
	else
		wood_bound_h = min(wood_bound_h, p->height*depth);
	return;
}

void wood_addanimtobg(flash_anim_t *p, int depth)
{
	int i;

	i = wood_addlayer(&bgstack, depth);
	bgstack.layersim[i] = NULL;
	bgstack.layersam[i] = p;
	bgstack.layerstm[i] = NULL;
	bgstack.depths[i] = depth;

	if(wood_bound_w == 0)
		wood_bound_w = p->frames[0]->width*depth;
	else
		wood_bound_w = min(wood_bound_w, p->frames[0]->width*depth);
	if(wood_bound_h == 0)
		wood_bound_h = p->frames[0]->height*depth;
	else
		wood_bound_h = min(wood_bound_h, p->frames[0]->height*depth);
	return;
}

void wood_addtilemaptobg(wood_tilemap_t *p, int depth)
{
	int i;

	i = wood_addlayer(&bgstack, depth);
	bgstack.layersim[i] = NULL;
	bgstack.layersam[i] = NULL;
	bgstack.layerstm[i] = p;
	bgstack.depths[i] = depth;

	if(wood_bound_w == 0)
		wood_bound_w = (p->width*p->tilew)*depth;
	else
		wood_bound_w = min(wood_bound_w, (p->width*p->tilew)*depth);
	if(wood_bound_h == 0)
		wood_bound_h = (p->height*p->tileh)*depth;
	else
		wood_bound_h = min(wood_bound_h, (p->height*p->tileh)*depth);
	return;
}

void wood_addspritetofg(flash_image_t *p, int depth)
{
	int i;

	i = wood_addlayer(&fgstack, depth);
	fgstack.layersim[i] = p;
	fgstack.layersam[i] = NULL;
	fgstack.layerstm[i] = NULL;
	fgstack.depths[i] = depth;

	if(wood_bound_w == 0)
		wood_bound_w = p->width/depth;
	else
		wood_bound_w = min(wood_bound_w, p->width/depth);
	if(wood_bound_h == 0)
		wood_bound_h = p->height/depth;
	else
		wood_bound_h = min(wood_bound_h, p->height/depth);
	return;
}

void wood_addtilemaptofg(wood_tilemap_t *p, int depth)
{
	int i;

	i = wood_addlayer(&fgstack, depth);
	fgstack.layersim[i] = NULL;
	fgstack.layersam[i] = NULL;
	fgstack.layerstm[i] = p;
	fgstack.depths[i] = depth;

	if(wood_bound_w == 0)
		wood_bound_w = (p->width*p->tilew)*depth;
	else
		wood_bound_w = min(wood_bound_w, (p->width*p->tilew)*depth);
	if(wood_bound_h == 0)
		wood_bound_h = (p->height*p->tileh)*depth;
	else
		wood_bound_h = min(wood_bound_h, (p->height*p->tileh)*depth);
	return;
}

void wood_updatebg(void)
{
	int i, j, k;
	wood_tilemap_t *p;

	if(bgstack.nlayers) {
		if(bgstack.layersim[0] != NULL) {
			air_vanillablit(bgstack.layersim[0],
			                -wood_camera_x/bgstack.depths[0],
			                -wood_camera_y/bgstack.depths[0]);
		} else if(bgstack.layersam[0] != NULL) {
			air_vanillablit(flash_animnextframe(bgstack.layersam[0]),
			                -wood_camera_x/bgstack.depths[0],
			                -wood_camera_y/bgstack.depths[0]);
		} else {
			p = bgstack.layerstm[0];
			for(k = wood_camera_y/p->tileh;
			    k <= (wood_camera_y+wood_h)/p->tileh && k < p->height;
			    k++) {
				for(j = wood_camera_x/p->tilew;
				    j <= (wood_camera_x+wood_w)/p->tilew && j < p->width;
				    j++) {
					air_vanillablit(p->tiles[p->map[k*p->width+j]],
					         j*p->tilew-wood_camera_x,
					         k*p->tileh-wood_camera_y);
				}
			}
		}
	}
	for(i = 1; i < bgstack.nlayers; i++) {
		if(bgstack.layersim[i] != NULL) {
			air_blit(bgstack.layersim[i],
			         -wood_camera_x/bgstack.depths[i],
			         -wood_camera_y/bgstack.depths[i]);
		} else if(bgstack.layersam[i] != NULL) {
			air_blit(flash_animnextframe(bgstack.layersam[0]),
			         -wood_camera_x/bgstack.depths[0],
			         -wood_camera_y/bgstack.depths[0]);
		} else {
			p = bgstack.layerstm[i];
			for(k = wood_camera_y/p->tileh;
			    k <= (wood_camera_y+wood_h)/p->tileh && k < p->height;
			    k++) {
				for(j = wood_camera_x/p->tilew;
				    j <= (wood_camera_x+wood_w)/p->tilew && j < p->width;
				    j++) {
					air_blit(p->tiles[p->map[k*p->width+j]],
					         j*p->tilew-wood_camera_x,
					         k*p->tileh-wood_camera_y);
				}
			}
		}
	}
}

void wood_updatefg(void)
{
	int i, j, k;
	wood_tilemap_t *p;

	for(i = 0; i < fgstack.nlayers; i++) {
		if(fgstack.layersim[i] != NULL) {
			air_blit(fgstack.layersim[i],
			         -wood_camera_x/fgstack.depths[i],
			         -wood_camera_y/fgstack.depths[i]);
		} else if(fgstack.layersam[i] != NULL) {
			air_blit(flash_animnextframe(fgstack.layersam[0]),
			         -wood_camera_x/fgstack.depths[0],
			         -wood_camera_y/fgstack.depths[0]);
		} else {
			p = fgstack.layerstm[i];
			for(k = wood_camera_y/p->tileh;
			    k <= (wood_camera_y+wood_h)/p->tileh;
			    k++) {
				for(j = wood_camera_x/p->tilew;
				    j <= (wood_camera_x+wood_w)/p->tilew;
				    j++) {
					air_blit(p->tiles[p->map[k*p->width+j]],
					         j*p->tilew-wood_camera_x,
					         k*p->tileh-wood_camera_y);
				}
			}
		}
	}
}

char wood_tiletype(int x, int y)
{
	int i;
	wood_tilemap_t *p;

	for(i = 0; i < bgstack.nlayers; i++)
		if(bgstack.layerstm[i] != NULL) {
			p = bgstack.layerstm[i];
			return p->map[(x+wood_camera_x)/p->tilew +
			              ((y+wood_camera_y)/p->tileh)*p->width];
		}
	return 0;
}

/* EOF scroll.c */
