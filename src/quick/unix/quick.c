/*
 * Timing routines for quick
 */

#include <quick.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct quick_handle_s {
	struct timeval end;
} quick_handle_t;

void *quick_start(int);
void quick_stop(void *);
void quick_wait(long);

void *quick_start(int fps)
{
	quick_handle_t *handle;
	int diff;

	handle = malloc(sizeof(quick_handle_t));
	gettimeofday(&handle->end, NULL);
	diff = 1e6/fps;
	if(diff+handle->end.tv_usec >= 1e6)
		handle->end.tv_sec++;
	handle->end.tv_usec = (handle->end.tv_usec+diff)%(int)1e6;

	return (void *)handle;
}

void quick_stop(void *handle_)
{
	struct timeval curtv;
	quick_handle_t *handle = (quick_handle_t *)handle_;

	gettimeofday(&curtv, NULL);
	if(curtv.tv_sec < handle->end.tv_sec ||
	   (curtv.tv_sec == handle->end.tv_sec &&
	    curtv.tv_usec < handle->end.tv_usec)) {
		quick_wait((handle->end.tv_sec-curtv.tv_sec)*1e6+
		           (handle->end.tv_usec-curtv.tv_usec));
	}
	free(handle);
	return;
}

void quick_wait(long usec)
{
	usleep(usec);
	return;
}

/* EOF quick.c */
