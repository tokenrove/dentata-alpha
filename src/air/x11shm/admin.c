/*
 * Administrative Functions of air
 * X11 w/MITSHM version
 */

#include <air.h>

#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>

#include "internal.h"

static XShmSegmentInfo shminfo;
static Colormap colormap;
static GC graphcont;
Display *air_x_display;
Window air_x_window;
XImage *air_vbuf;
unsigned char *air_palette;
int air_mode_w, air_mode_h, air_mode_type;

int air_init(int w, int h, int type)
{
	Screen *screen;
	XColor color;
	int i, j, depth;

	air_mode_w = w;
	air_mode_h = h;
	air_mode_type = type;

/* Open the display */
	air_x_display = XOpenDisplay(NULL);
	if(air_x_display == NULL) {
		return 0;
	}
/* Get the GC and Colormap and whatnot... */
	screen = DefaultScreenOfDisplay(air_x_display);
	graphcont = DefaultGCOfScreen(screen);
	colormap = DefaultColormapOfScreen(screen);
	depth = DefaultDepthOfScreen(screen);
	if(air_mode_type&AIR_8BPP && depth != 8)
		return 0;
/* Open the window */
	air_x_window = XCreateSimpleWindow(air_x_display,
	                             RootWindowOfScreen(screen), 0, 0,
				     air_mode_w, air_mode_h, 1,
	                             WhitePixelOfScreen(screen),
	                             BlackPixelOfScreen(screen));
	if(air_x_window == 0) {
		return 0;
	}
/* Change title bar name */
	XStoreName(air_x_display, air_x_window, "dentata demo");
/* Create the XImage */
	air_vbuf = XShmCreateImage(air_x_display,
	                           DefaultVisualOfScreen(screen),
	                           depth, ZPixmap, NULL, &shminfo,
	                           air_mode_w, air_mode_h);
	if(air_vbuf == NULL) {
		return 0;
	}
/* Get SHM info */
	shminfo.shmid = shmget(IPC_PRIVATE,
	                       air_vbuf->bytes_per_line*air_vbuf->height,
	                       IPC_CREAT|0777);
	if(shminfo.shmid < 0) {
		return 0;
	}
	shminfo.shmaddr=shmat(shminfo.shmid, 0, 0);
	air_vbuf->data=shminfo.shmaddr;
	if(air_vbuf->data == NULL) {
		return 0;
	}
	shminfo.readOnly = False;
	if(XShmAttach(air_x_display, &shminfo) == 0) {
		return 0;
	}
	XMapRaised(air_x_display, air_x_window);
	XClearWindow(air_x_display, air_x_window);
	XFlush(air_x_display);
	return 1;
}

void air_close(void)
{
	if(air_x_display) {
		if(air_x_window) {
			XShmDetach(air_x_display, &shminfo);
			if(shminfo.shmaddr)
				shmdt(shminfo.shmaddr);
			if(shminfo.shmid >= 0)
				shmctl(shminfo.shmid, IPC_RMID, 0);
		}
		XCloseDisplay(air_x_display);
	}
	return;
}

void air_update(void)
{
	XShmPutImage(air_x_display, air_x_window, graphcont, air_vbuf,
	             0, 0, 0, 0, air_mode_w, air_mode_h, False);
	XFlush(air_x_display);
}

void air_setpalette(unsigned char *palette)
{
	int i;
	XColor color;
	unsigned long foo[256], bar[1];
	Visual visual;

	visual = *DefaultVisual(air_x_display, DefaultScreen(air_x_display));
	colormap = XCreateColormap(air_x_display, air_x_window, &visual,
	                           AllocAll);
	for(i = 0; i < 256; i++) {
		color.flags = DoRed|DoGreen|DoBlue;
		color.red = palette[3*i+0]<<10;
		color.green = palette[3*i+1]<<10;
		color.blue = palette[3*i+2]<<10;
		color.pixel = i;
		XStoreColor(air_x_display, colormap, &color);
	}
	XSetWindowColormap(air_x_display, air_x_window, colormap);
	XInstallColormap(air_x_display, colormap);
	air_palette = palette;
	return;
}

unsigned char *air_getpalette(void)
{
	return air_palette;
}

/* EOf admin.c */
