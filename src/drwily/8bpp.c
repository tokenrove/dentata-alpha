
#include <drwily.h>

#define min(x, y) (((x) > (y)) ? (y) : (x))
#define max(x, y) (((x) < (y)) ? (y) : (x))

void drwily_blit8(char *dest, int dw, int dh, char *src, int sw, int sh,
                  int x, int y);
void drwily_blit8noalpha(char *dest, int dw, int dh, char *src, int sw, int sh,
                         int x, int y);

void drwily_blit8(char *dest, int dw, int dh, char *src, int sw, int sh,
                  int x, int y)
{
	register int i, j;
	int maxy, maxx;

	if(x > dw || x+sw < 0 ||
           y > dh || y+sh < 0) return;

	maxy = max(0, -y);
	maxx = max(0, -x);
	src += sw*maxy+maxx;
	dest += dw*(maxy+y)+(maxx+x);
	for(i = maxy; i < sh && i+y < dh; i++) {
		for(j = maxx; j < sw && j+x < dw; j++) {
			if(*src != 0)
				*dest = *src;
			src++; dest++;
		}
		dest += (dw-(j+x))+(maxx+x);
		src += (sw-j)+maxx;
	}
	return;
}

void drwily_blit8noalpha(char *dest, int dw, int dh, char *src, int sw, int sh,
                         int x, int y)
{
	int i;

	if(x > dw || x+sw < 0 ||
           y > dh || y+sh < 0) return;

	for(i = max(0, -y); i < sh && i+y < dh; i++)
		memcpy(dest+(i+y)*dw+x+max(0,-x), src+i*sw+max(0,-x),
		       min(sw-max(0,-x), dw-(x+max(0,-x))));
	return;
}
