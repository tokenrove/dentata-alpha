
CFLAGS=-Wall -pedantic -g -I../include

default: test3$(OUT)

test3$(OUT): test3.o
	gcc $(CFLAGS) test3.o -o test3$(OUT) -L../lib -lair -lquick -lflash -lmetal -lcrash -lwood -lbubble -lheat -ldrwily $(LIBS)

clean:
	rm -f test3.exe test3
