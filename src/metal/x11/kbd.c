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
	return 1;
}

void metal_update(void)
{
	XCheckWindowEvent(air_x_display, air_x_window,
	                  KeyPressMask|KeyReleaseMask, &event);
}

int metal_ishit(int code)
{
	unsigned short foo;

	foo = XLookupKeysym((XKeyEvent *)&event, 0);
	if(event.type == KeyPress)
		return (foo == metal_transtable[code]);
	return 0;
}

void metal_close(void)
{
	XAutoRepeatOn(air_x_display);
}

/* EOF kbd.c */
