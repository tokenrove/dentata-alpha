
CCFLAGS=-Wall -g
CFLAGS=$(CCFLAGS) -I../include

all: repal$(OUT) sprcomp$(OUT)

repal$(OUT): repal.o
	$(CC) $(CFLAGS) repal.o -o repal$(OUT) -L../lib -lflash -ldrwily

sprcomp$(OUT): sprcomp.o
	$(CC) $(CFLAGS) sprcomp.o -o sprcomp$(OUT)
