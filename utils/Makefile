
CCFLAGS=-Wall -g
CFLAGS=$(CCFLAGS) -I../include

all: repal$(OUT) sprcomp$(OUT) tmcomp$(OUT)

repal$(OUT): repal.o
	$(CC) $(CFLAGS) repal.o -o repal$(OUT) -L../lib -lflash -lair -ldrwily $(LIBS)

sprcomp$(OUT): sprcomp.o
	$(CC) $(CFLAGS) sprcomp.o -o sprcomp$(OUT)

tmcomp$(OUT): tmcomp.o
	$(CC) $(CFLAGS) tmcomp.o -o tmcomp$(OUT) -L../lib -lflash -lair -ldrwily $(LIBS)
