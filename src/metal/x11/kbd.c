/*
 * Keyboard functions for metal
 * X11 version
 */

#include <metal.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

extern Display *air_x_display;
extern Window air_x_window;
static XEvent event;

static int metal_transtable[METAL_K_LAST] = { 0, XK_Escape,
	XK_Up, XK_Down, XK_Left, XK_Right, XK_Return
};
static char metal_map[METAL_K_LAST];

int metal_init(void);
void metal_update(void);
int metal_ishit(int);
void metal_close(void);

int metal_init(void)
{
	XSelectInput(air_x_display, air_x_window,
	             FocusChangeMask|ExposureMask|KeyPressMask|KeyReleaseMask);
	XAutoRepeatOff(air_x_display);
	XWindowEvent(air_x_display, air_x_window, ExposureMask, &event);
	memset(metal_map, 0, METAL_K_LAST);
	return 1;
}

void metal_update(void)
{
	int i, j;
	while(XCheckWindowEvent(air_x_display, air_x_window,
	                        KeyPressMask|KeyReleaseMask, &event) == True) {
		j = XLookupKeysym((XKeyEvent *)&event, 0);
		if(event.type == KeyPress) {
			for(i = 0; i < METAL_K_LAST; i++)
				if(metal_transtable[i] == j) {
					metal_map[i] = 1;
					break;
				}
		}
		else if(event.type == KeyRelease) {
			for(i = 0; i < METAL_K_LAST; i++)
				if(metal_transtable[i] == j) {
					metal_map[i] = 0;
					break;
				}
		}
	}
	return;
}

int metal_ishit(int code)
{
	return metal_map[code];
}

void metal_close(void)
{
	XAutoRepeatOn(air_x_display);
}

/* EOF kbd.c */
