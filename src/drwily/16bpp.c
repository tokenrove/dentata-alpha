/*
 * 16bpp blit functions for drwily
 */

#include <drwily.h>
#include <string.h>

#define min(x, y) (((x) > (y)) ? (y) : (x))
#define max(x, y) (((x) < (y)) ? (y) : (x))

void drwily_blit16alpha1(char *dest, int dw, int dh, char *src,
                         char *alpha, int sw, int sh, int x, int y);
void drwily_blit16alpha8(char *dest, int dw, int dh, char *src,
                         char *alpha, int sw, int sh, int x, int y);
void drwily_blit16noalpha(char *dest, int dw, int dh, char *src, int sw,
                          int sh, int x, int y);

void drwily_blit16noalpha(char *dest, int dw, int dh, char *src, int sw,
                          int sh, int x, int y)
{
	int i;

	if(x > dw || x+sw < 0 ||
           y > dh || y+sh < 0) return;

	for(i = max(0, -y); i < sh && i+y < dh; i++)
		memcpy(dest+2*((i+y)*dw+x+max(0,-x)), src+2*(i*sw+max(0,-x)),
		       2*min(sw-max(0,-x), dw-(x+max(0,-x))));
	return;
}

void drwily_blit16alpha1(char *dest, int dw, int dh, char *src,
                         char *alpha, int sw, int sh, int x, int y)
{
	register int i, j;
	int maxy, maxx;

	if(x > dw || x+sw < 0 ||
           y > dh || y+sh < 0) return;

	maxy = max(0, -y);
	maxx = max(0, -x);
	src += 2*(sw*maxy+maxx);
	dest += 2*(dw*(maxy+y)+(maxx+x));
	for(i = maxy; i < sh && i+y < dh; i++) {
		for(j = maxx; j < sw && j+x < dw; j++) {
			if(alpha[(i*sw+j)/8]&(1<<((i*sw+j)%8))) {
				*dest = *src;
				*(dest+1) = *(src+1);
			}
			src+=2; dest+=2;
		}
		dest += 2*((dw-(j+x))+(maxx+x));
		src += 2*((sw-j)+maxx);
	}
	return;
}

void drwily_blit16alpha8(char *dest, int dw, int dh, char *src,
                         char *alpha, int sw, int sh, int x, int y)
{
	register int i, j;
	int maxy, maxx;
	unsigned char sr, sg, sb, dr, dg, db;

	if(x > dw || x+sw < 0 ||
           y > dh || y+sh < 0) return;

	maxy = max(0, -y);
	maxx = max(0, -x);
	src += 2*(sw*maxy+maxx);
	dest += 2*(dw*(maxy+y)+(maxx+x));
	for(i = maxy; i < sh && i+y < dh; i++) {
		for(j = maxx; j < sw && j+x < dw; j++) {
			sb = *src<<3; sg = *src>>5; src++;
			sr = (*src>>3)<<3; sg |= (*src&7)<<5; src++;
			db = *dest<<3; dg = *dest>>5;
			dr = (*(dest+1)>>3)<<3; dg |= (*(dest+1)&7)<<5;

			dr = ((sr*(alpha[i*sw+j]+1))>>8)+((dr*((255-alpha[i*sw+j])+1))>>8);
			dg = ((sg*(alpha[i*sw+j]+1))>>8)+((dg*((255-alpha[i*sw+j])+1))>>8);
			db = ((sb*(alpha[i*sw+j]+1))>>8)+((db*((255-alpha[i*sw+j])+1))>>8);
			*dest = ((dg<<3)&~63)|(db>>3);
			dest++;
			*dest = (dr&~7)|(dg>>5);
			dest++;
		}
		dest += 2*((dw-(j+x))+(maxx+x));
		src += 2*((sw-j)+maxx);
	}
	return;
}

/* EOF 16bpp.c */
