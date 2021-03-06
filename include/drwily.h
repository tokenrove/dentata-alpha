/*
 * Internal library functions
 */

#ifndef DRWILY_H
#define DRWILY_H

extern void drwily_blit8(char *dest, int dw, int dh, char *src, int sw,
                         int sh, int x, int y);
extern void drwily_blit8noalpha(char *dest, int dw, int dh, char *src, int sw,
                                int sh, int x, int y);
extern void drwily_blit16alpha1(char *dest, int dw, int dh, char *src,
                                char *alpha, int sw, int sh, int x, int y);
extern void drwily_blit16alpha8(char *dest, int dw, int dh, char *src,
                                char *alpha, int sw, int sh, int x, int y);
extern void drwily_blit16noalpha(char *dest, int dw, int dh, char *src, int sw,
                                 int sh, int x, int y);

#endif

/* EOF drwily.h */
