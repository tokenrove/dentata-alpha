/*
 * Keyboard functions for metal
 * DOS version
 */

#include <metal.h>

#include <dpmi.h>
#include <go32.h>
#include <dos.h>
#include <pc.h>

static int metal_transtable[METAL_K_LAST] = { 0, 1,
	72, 80, 75, 77, 28
};

int metal_init(void);
void metal_update(void);
int metal_ishit(int);
void metal_close(void);

#define METAL_KTABLELEN 512

static volatile char metal_ktable[METAL_KTABLELEN];
static volatile unsigned char metal_c;

static __dpmi_paddr oldint9;
static _go32_dpmi_seginfo seginfo;

/*
 * extern void metal_rm_lowlevel(void);
 * extern void metal_rm_lowlevel_end(void);
 */

static void metal_pm_lowlevel(void)
{
	metal_c = inportb(0x60);
	if(metal_c == 0xE0) { /* extended */
		goto end;
	}
	if(metal_c&0x80) {
		metal_ktable[metal_c&0x7F] = 0;
	} else {
		metal_ktable[metal_c&0x7F] = 1;
	}
end:
	outportb(0x20, 0x20);
	return;
}

void metal_pm_lowlevel_end(void) { }

int metal_init(void)
{
	__dpmi_paddr newint9;

	newint9.selector = _my_cs();
	newint9.offset32 = (long)metal_pm_lowlevel;

	seginfo.pm_offset = (long)metal_pm_lowlevel;

	/* install handler */
#if 0
	_go32_dpmi_lock_code(metal_pm_lowlevel,
	                     (unsigned long)(metal_pm_lowlevel_end-
	                                     metal_pm_lowlevel));
/*	_go32_dpmi_lock_code(metal_rm_lowlevel,
	                     (unsigned long)(metal_rm_lowlevel_end-
	                                     metal_rm_lowlevel)); */
	_go32_dpmi_lock_data(metal_ktable, METAL_KTABLELEN);
	_go32_dpmi_lock_data(metal_c, 1);
#endif
	__dpmi_get_protected_mode_interrupt_vector(9, &oldint9);
	_go32_dpmi_allocate_iret_wrapper(&seginfo);
	_go32_dpmi_set_protected_mode_interrupt_vector(9, &seginfo);
/*	__dpmi_set_protected_mode_interrupt_vector(9, &newint9); */
	return 1;
}

void metal_update(void)
{
	return;
}

int metal_ishit(int code)
{
	return metal_ktable[metal_transtable[code]];
}

void metal_close(void)
{
	/* remove handler */
	__dpmi_set_protected_mode_interrupt_vector(9, &oldint9);
	_go32_dpmi_free_iret_wrapper(&seginfo);
	return;
}

/* EOF kbd.c */
