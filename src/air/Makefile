
ifeq ($(TARGET), svgalib)
FOO=svgalib
endif
ifeq ($(TARGET), x11)
FOO=x11shm
endif
ifeq ($(TARGET), dos)
FOO=dos
endif

default:
	make -C $(FOO)
