/*
 * Keyboard functions for metal
 */

#include <vgakeyboard.h>

#include <metal.h>

static int metal_transtable[METAL_K_LAST] = { 0, SCANCODE_ESCAPE,
	SCANCODE_CURSORBLOCKUP,	SCANCODE_CURSORBLOCKDOWN,
	SCANCODE_CURSORBLOCKLEFT, SCANCODE_CURSORBLOCKRIGHT,
	SCANCODE_ENTER
};

int metal_init(void);
void metal_update(void);
int metal_ishit(int);
void metal_close(void);

int metal_init(void)
{
	if(keyboard_init() != 0)
		return 0;
	return 1;
}

void metal_update(void)
{
	keyboard_update();
}

int metal_ishit(int code)
{
	return keyboard_keypressed(metal_transtable[code]);
}

void metal_close(void)
{
	keyboard_close();
}

/* EOF kbd.c */
