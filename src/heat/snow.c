/*
 * Snow routines for heat
 */

#include <heat.h>
#include <air.h>
#include <flash.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef struct heat_snow_s {
	flash_image_t *plane;
	int nflakes, nactive;
	int *flake_x;
	int *flake_y;
	int white;
} heat_snow_t;

void *heat_snowinit(int);
void heat_snowupdate(void *);
void heat_snowclose(void *);

void *heat_snowinit(int nflakes)
{
	heat_snow_t *snow;

	snow = malloc(sizeof(heat_snow_t));
	snow->plane = flash_imgnew(air_mode_w, air_mode_h, AIR_8BPP);
	snow->nflakes = nflakes;
	snow->nactive = 0;
	snow->flake_x = malloc(nflakes*sizeof(int));
	snow->flake_y = malloc(nflakes*sizeof(int));
	snow->white = flash_closestcolor(255,255,255,air_getpalette());
	srand(time(NULL));

	return snow;
}

void heat_snowupdate(void *snow_)
{
	heat_snow_t *snow = (heat_snow_t *)snow_;
	int i;

	memset(snow->plane->data, 0, snow->plane->width*snow->plane->height);

	for(i = 0; i < snow->nactive; i++) {
		if((float)rand()/(RAND_MAX+1.0) > 0.5) snow->flake_y[i]++;
		snow->flake_y[i]++;

		if(snow->flake_y[i] >= snow->plane->height) {
			memmove(&snow->flake_x[i], &snow->flake_x[i+1],
				(snow->nactive-1-i)*sizeof(int));
			memmove(&snow->flake_y[i], &snow->flake_y[i+1],
				(snow->nactive-1-i)*sizeof(int));
			snow->nactive--; i--;
			continue;
		}

		if((float)rand()/(RAND_MAX+1.0) < 0.33) snow->flake_x[i]++;
		if((float)rand()/(RAND_MAX+1.0) < 0.33) snow->flake_x[i]--;

		if(snow->flake_x[i] >= snow->plane->width)
			snow->flake_x[i] = snow->plane->width;
		if(snow->flake_x[i] < 0)
			snow->flake_x[i] = 0;

		snow->plane->data[snow->flake_x[i]+
		            snow->flake_y[i]*snow->plane->width] = snow->white;
	}

	if(snow->nactive < snow->nflakes &&
	   (float)rand()/(RAND_MAX+1.0) > 0.3) {
		snow->flake_x[snow->nactive] = (int)((float)snow->plane->width*rand()/(RAND_MAX+1.0));
		snow->flake_y[snow->nactive] = 0;
		snow->nactive++;
	}
	air_blit(snow->plane, 0, 0);
	return;
}

void heat_snowclose(void *snow_)
{
	heat_snow_t *snow = (heat_snow_t *)snow_;

	flash_imgdelete(snow->plane);
	free(snow->flake_x);
	free(snow->flake_y);
	free(snow);
	return;
}

/* EOF snow.c */
