
CCFLAGS=-Wall -g
CFLAGS=$(CCFLAGS) -I../include

all: repalette sprcomp

repalette: repalette.o
	$(CC) $(CFLAGS) repalette.o -o repalette -L../lib -lflash -ldrwily

sprcomp: sprcomp.o
	$(CC) $(CFLAGS) sprcomp.o -o sprcomp
