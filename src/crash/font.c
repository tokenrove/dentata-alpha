/*
 * Font components of crash
 */

#include <crash.h>
#include <flash.h>
#include <air.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

crash_font_t *crash_loadrawfont(char *, int, int, int);
void crash_printf(int, int, crash_font_t *, char *, ...);
void crash_sprintf(flash_image_t *, int, int, crash_font_t *,
                   char *, ...);
void crash_delete(crash_font_t *);

crash_font_t *crash_loadrawfont(char *filename, int w, int h, int c)
{
	FILE *fp;
	crash_font_t *p;
	char *buffer;
	int i, j;

	fp = fopen(filename, "rb");
	if(fp == NULL) return NULL;

	p = malloc(sizeof(crash_font_t));
	if(p == NULL) return NULL;

	p->width = w;
	p->height = h;
	buffer = malloc(p->height);

	for(i = 0; i < CRASH_NCHARS; i++) {
		p->chars[i] = flash_imgnew(p->width, p->height, AIR_8BPP);
		fread(buffer, p->height, 1, fp);
		for(j = 0; j < p->width*p->height; j++) {
			if(buffer[j/p->width]&(1<<(7-(j%p->width))))
				p->chars[i]->data[j] = c;
			else
				p->chars[i]->data[j] = 0;
		}
	}

	free(buffer);
	fclose(fp);
	return p;
}

int crash_getlen(char *fmt, va_list args)
{
	int i, j, len;
	char *p;

	if(fmt == NULL)
		fmt = "(null)";

	len = 0;
	for(i = 0; i < strlen(fmt); i++) {
		if(fmt[i] == '%') {
			i++;
			if(fmt[i] == 'd' || fmt[i] == 'i') {
				j = va_arg(args, int);
				if(j == 0) len++;
				if(j < 0) { len++; j = -j; }
				while(j > 0) { j/=10; len++; }
			} else if(fmt[i] == 's') {
				p = va_arg(args, char *);
				if(p == NULL) len += 6;
				else len += strlen(p);
			} else if(fmt[i] == 'c') {
				j = va_arg(args, int);
				len++;
			} else if(fmt[i] == '%') {
				len++;
			} else {
				fprintf(stderr, "Uknown format %c\n", fmt[i]);
			}
		} else {
			len++;
		}
	}
	return len;
}

void crash_printf(int x, int y, crash_font_t *fnt, char *fmt, ...)
{
	va_list args;
	char *p;
	int i, len;

	va_start(args, fmt);
	len = crash_getlen(fmt, args);
	p = malloc(len+1);
	va_start(args, fmt);
	vsprintf(p, fmt, args);
	for(i = 0; i < strlen(p); i++) {
		air_blit(fnt->chars[p[i]], x, y);
		x += fnt->width+1;
	}
	free(p);
	va_end(args);
	return;
}

void crash_sprintf(flash_image_t *dest, int x, int y, crash_font_t *fnt,
                   char *fmt, ...)
{
	va_list args;
	char *p;
	int i, len;

	va_start(args, fmt);
	len = crash_getlen(fmt, args);
	p = malloc(len+1);
	va_start(args, fmt);
	vsprintf(p, fmt, args);
	for(i = 0; i < strlen(p); i++) {
		flash_imgblit(dest, fnt->chars[p[i]], x, y);
		x += fnt->width+1;
	}
	free(p);
	va_end(args);
	return;
}

void crash_delete(crash_font_t *p)
{
	int i;

	for(i = 0; i < CRASH_NCHARS; i++)
		flash_imgdelete(p->chars[i]);
	free(p);
	return;
}

/* EOF font.c */
