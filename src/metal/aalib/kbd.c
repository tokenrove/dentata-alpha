/*
 * Keyboard functions for metal
 */

#include <aalib.h>

#include <metal.h>

static int metal_transtable[METAL_K_LAST] = { 0, 'q',
	'w', 's', 'a', 'd', '\''
};
static char metal_map[METAL_K_LAST];

extern aa_context *air_context;

int metal_init(void);
void metal_update(void);
int metal_ishit(int);
void metal_close(void);

int metal_init(void)
{
	aa_autoinitkbd(air_context, AA_SENDRELEASE);
	memset(metal_map, 0, METAL_K_LAST);
	return 1;
}

void metal_update(void)
{
	int ev, i;

	for(i = 0; i < METAL_K_LAST; i++)
		metal_map[i] = 0;

	while(ev = aa_getevent(air_context, 0), ev != AA_NONE) {
		if(ev > AA_RELEASE) {
			for(i = 0; i < METAL_K_LAST; i++)
				if(metal_transtable[i] == ev) {
					metal_map[i] = 0;
					break;
				}
		} else {
			for(i = 0; i < METAL_K_LAST; i++)
				if(metal_transtable[i] == ev) {
					metal_map[i] = 1;
					break;
				}
		}
	}
}

int metal_ishit(int code)
{
	return metal_map[code];
}

void metal_close(void)
{
	aa_uninitkbd(air_context);
}

/* EOF kbd.c */
