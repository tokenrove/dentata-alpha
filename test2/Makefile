
CFLAGS=-Wall -pedantic -g -I../include

default: test2$(OUT)

test2$(OUT): test2.o
	gcc $(CFLAGS) test2.o -o test2$(OUT) -L../lib -lair -lquick -lmetal -lwood -lcrash -lflash -ldrwily $(LIBS)

clean:
	rm -f test2.exe test2
